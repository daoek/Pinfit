<#
    Installs Pinfit into a per-user directory (default %LOCALAPPDATA%\Pinfit). No admin rights
    are required and nothing outside the install directory is touched, other than adding that
    directory to the current user's PATH.

    One-line install of the newest release, from any PowerShell prompt:

        irm https://raw.githubusercontent.com/daoek/Pinfit/main/scripts/install.ps1 | iex

    What gets installed is self-contained: pinfit.jar plus its own trimmed Java runtime and a
    launcher that only uses that runtime. No Java has to be installed on the machine.

    Two modes:
      - Pass -Version <tag> (e.g. "0.1.0" - see the Releases page for what's published),
        or -Version latest, to download that release's Windows bundle and SHA256SUMS from GitHub
        over HTTPS, verify the checksum, and install only if it matches. Nothing is written to
        the install directory if verification fails. This is also the default whenever the
        script is not run from a repository checkout (e.g. via irm | iex).
      - Run from a checkout without -Version to build the bundle instead
        (`mvn -Pbundle clean package`, which needs a JDK 17+ with jlink), which is what the
        "Pinfit: Package + Install" VS Code task and contributors use.

    A note on -ExecutionPolicy Bypass, since it shows up in the recommended invocation: that
    flag scopes to the single powershell.exe process it's passed to. It does not change the
    machine's or the current user's execution policy - running this script that way leaves
    every other script on the machine subject to whatever policy was already in effect.
#>
[CmdletBinding()]
param(
    [string]$InstallDirectory = (Join-Path $env:LOCALAPPDATA 'Pinfit'),
    [string]$Version,
    [switch]$SkipBuild,
    [switch]$SkipPathUpdate
)

# Everything runs in a child scope: piped into iex, this script otherwise executes in the caller's
# own session and would leave StrictMode and ErrorActionPreference changed there afterwards.
& {
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if ($Version -and $SkipBuild) {
    throw '-Version and -SkipBuild are mutually exclusive: -Version installs a downloaded release and never builds locally.'
}

$repositorySlug = 'daoek/Pinfit'
# Empty when the script is piped into iex - there is then no checkout to build from.
$repositoryRoot = if ($PSScriptRoot) { Split-Path -Parent $PSScriptRoot } else { $null }
$inCheckout = $repositoryRoot -and (Test-Path -LiteralPath (Join-Path $repositoryRoot 'pom.xml') -PathType Leaf)
if (-not $Version -and -not $inCheckout) {
    if ($SkipBuild) {
        throw '-SkipBuild needs a repository checkout; run install.ps1 from inside one.'
    }
    $Version = 'latest'
}
$resolvedInstallDirectory = [System.IO.Path]::GetFullPath($InstallDirectory)
$pathRoot = [System.IO.Path]::GetPathRoot($resolvedInstallDirectory)
$markerName = '.pinfit-install-marker'
$markerPath = Join-Path $resolvedInstallDirectory $markerName

if ($resolvedInstallDirectory -eq $pathRoot -or
    $resolvedInstallDirectory -eq [System.IO.Path]::GetFullPath($env:LOCALAPPDATA) -or
    $resolvedInstallDirectory -eq [System.IO.Path]::GetFullPath($env:USERPROFILE)) {
    throw "Refusing unsafe installation directory: $resolvedInstallDirectory"
}

if (Test-Path -LiteralPath $resolvedInstallDirectory) {
    $hasContent = (Get-ChildItem -Force -LiteralPath $resolvedInstallDirectory | Measure-Object).Count -gt 0
    if ($hasContent -and -not (Test-Path -LiteralPath $markerPath -PathType Leaf)) {
        throw "Refusing to overwrite non-Pinfit directory: $resolvedInstallDirectory"
    }
}

function Assert-Sha256Match {
    # Verifies $FilePath's SHA-256 against the entry for $FileName in a SHA256SUMS file
    # (the standard "<hash>  <filename>" format). Throws - and installs nothing - on any
    # mismatch or missing entry, so a corrupted or tampered download never gets installed.
    param(
        [Parameter(Mandatory)] [string]$FilePath,
        [Parameter(Mandatory)] [string]$FileName,
        [Parameter(Mandatory)] [string]$Sha256SumsPath
    )
    $expectedLine = Get-Content -LiteralPath $Sha256SumsPath |
        Where-Object { $_ -match ('^\s*[0-9a-fA-F]{64}\s+\*?' + [regex]::Escape($FileName) + '\s*$') } |
        Select-Object -First 1
    if (-not $expectedLine) {
        throw "No checksum entry for '$FileName' found in $Sha256SumsPath"
    }
    $expectedHash = ($expectedLine.Trim() -split '\s+')[0].ToLowerInvariant()
    $actualHash = (Get-FileHash -Algorithm SHA256 -LiteralPath $FilePath).Hash.ToLowerInvariant()
    if ($actualHash -ne $expectedHash) {
        throw "Checksum mismatch for '$FileName': expected $expectedHash, got $actualHash. Refusing to install."
    }
    Write-Output "Checksum verified for $FileName ($actualHash)"
}

function Resolve-LatestTag {
    # Newest stable release; before the first stable one exists, the newest prerelease.
    # github.com/.../releases/latest redirects to the newest stable release's tag page. Unlike the
    # REST API it is not rate-limited (60 calls/hour per IP, easily hit behind a shared office IP).
    try {
        $request = [System.Net.WebRequest]::Create("https://github.com/$repositorySlug/releases/latest")
        $request.Method = 'HEAD'
        $request.AllowAutoRedirect = $false
        $response = $request.GetResponse()
        try {
            $location = $response.Headers['Location']
        } finally {
            $response.Close()
        }
        if ($location -match '/releases/tag/([^/]+)$') {
            return $Matches[1]
        }
    } catch {
        # Fall through to the API below.
    }
    # No stable release yet: the API lists prereleases too.
    $headers = @{ 'User-Agent' = 'pinfit-installer' }
    $releases = @(Invoke-RestMethod -Uri "https://api.github.com/repos/$repositorySlug/releases?per_page=1" -Headers $headers)
    if ($releases.Count -eq 0) {
        throw "No published release found at https://github.com/$repositorySlug/releases"
    }
    return $releases[0].tag_name
}

if ($Version -eq 'latest') {
    try {
        $Version = Resolve-LatestTag
    } catch {
        throw "Could not look up the latest release of $repositorySlug`: $($_.Exception.Message)"
    }
    Write-Output "Latest release: $Version"
}

if ($Version) {
    $tag = if ($Version.StartsWith('v')) { $Version } else { "v$Version" }
    $bareVersion = $tag.TrimStart('v')
    # Windows on ARM runs this x64 bundle under emulation, so one Windows bundle serves both.
    $bundleName = "pinfit-$bareVersion-windows-x64.zip"
    $releaseBaseUrl = "https://github.com/$repositorySlug/releases/download/$tag"

    $downloadDirectory = Join-Path ([System.IO.Path]::GetTempPath()) "pinfit-install-$tag"
    if (Test-Path -LiteralPath $downloadDirectory) {
        Remove-Item -Recurse -Force -LiteralPath $downloadDirectory
    }
    New-Item -ItemType Directory -Force -Path $downloadDirectory | Out-Null
    $downloadedBundle = Join-Path $downloadDirectory $bundleName
    $downloadedSums = Join-Path $downloadDirectory 'SHA256SUMS'

    Write-Output "Downloading $bundleName from release $tag..."
    try {
        Invoke-WebRequest -Uri "$releaseBaseUrl/$bundleName" -OutFile $downloadedBundle -UseBasicParsing
        Invoke-WebRequest -Uri "$releaseBaseUrl/SHA256SUMS" -OutFile $downloadedSums -UseBasicParsing
    } catch {
        throw ("Could not download $bundleName from release '$tag' (https://github.com/$repositorySlug/releases). " +
            "Releases before the bundled Java runtime was introduced have no such file - install a newer version. " +
            $_.Exception.Message)
    }
    Assert-Sha256Match -FilePath $downloadedBundle -FileName $bundleName -Sha256SumsPath $downloadedSums
    $bundleDirectory = Join-Path $downloadDirectory 'bundle'
    Expand-Archive -LiteralPath $downloadedBundle -DestinationPath $bundleDirectory
} else {
    if (-not $SkipBuild) {
        $maven = Get-Command mvn.cmd -ErrorAction SilentlyContinue
        if ($null -eq $maven) {
            throw 'Maven (mvn.cmd) was not found on PATH.'
        }
        # -Pbundle adds the jlink runtime and launcher (target\bundle). Clean first so Shade never
        # consumes a JAR that was already shaded by a prior build.
        & $maven.Source -f (Join-Path $repositoryRoot 'pom.xml') -Pbundle clean package
        if ($LASTEXITCODE -ne 0) {
            throw "Maven build failed with exit code $LASTEXITCODE"
        }
    }
    $bundleDirectory = Join-Path $repositoryRoot 'target\bundle'
}

foreach ($required in 'pinfit.jar', 'pinfit.cmd', 'runtime\bin\java.exe') {
    if (-not (Test-Path -LiteralPath (Join-Path $bundleDirectory $required) -PathType Leaf)) {
        throw "The Pinfit bundle in $bundleDirectory is incomplete (no $required). Build it with 'mvn -Pbundle package', or run without -SkipBuild."
    }
}

# Replace the whole installation, runtime included, so no file from an older version lingers. The
# marker check above already proved this directory is Pinfit's own.
New-Item -ItemType Directory -Force -Path $resolvedInstallDirectory | Out-Null
Get-ChildItem -Force -LiteralPath $resolvedInstallDirectory |
    Where-Object { $_.Name -ne $markerName } |
    Remove-Item -Recurse -Force
Copy-Item -Recurse -Force -Path (Join-Path $bundleDirectory '*') -Destination $resolvedInstallDirectory
Set-Content -LiteralPath $markerPath -Value 'Pinfit managed installation. Safe removal requires this marker.' -Encoding utf8

if (-not $SkipPathUpdate) {
    $userPath = [Environment]::GetEnvironmentVariable('Path', 'User')
    $pathParts = @($userPath -split ';' | Where-Object { -not [string]::IsNullOrWhiteSpace($_) })
    $alreadyPresent = $pathParts | Where-Object {
        [System.IO.Path]::GetFullPath($_).TrimEnd('\') -ieq $resolvedInstallDirectory.TrimEnd('\')
    }
    if (-not $alreadyPresent) {
        $newUserPath = (@($pathParts) + $resolvedInstallDirectory) -join ';'
        [Environment]::SetEnvironmentVariable('Path', $newUserPath, 'User')
    }
    if (-not (($env:Path -split ';') -contains $resolvedInstallDirectory)) {
        $env:Path = $resolvedInstallDirectory + ';' + $env:Path
    }
}

$installedHash = (Get-FileHash -Algorithm SHA256 -LiteralPath (Join-Path $resolvedInstallDirectory 'pinfit.jar')).Hash.ToLowerInvariant()
Write-Output "Pinfit installed in $resolvedInstallDirectory"
Write-Output "Installed jar SHA256: $installedHash"
if (-not $SkipPathUpdate) {
    Write-Output 'Open a new terminal, then run: pinfit --help'
}
}
