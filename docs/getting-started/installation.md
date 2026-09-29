# Installation

Pinfit installs as a self-contained bundle for Windows (x64), Linux (x64 and arm64) and macOS (Intel
and Apple silicon). **You do not need Java, Maven or anything else installed.**

!!! info "Java is bundled, not required"

    Pinfit is written in Java, but every release bundle carries its own trimmed Java runtime (about
    20 MB, made with `jlink`), and the `pinfit` launcher only ever uses that runtime. A Java you may
    already have, `JAVA_HOME` and `PATH` make no difference to it — installing or upgrading some
    other Java can't break Pinfit, and Pinfit never touches yours.

## Install a release (recommended)

Every version tag is built and tested by
[CI](https://github.com/daoek/Pinfit/blob/main/.github/workflows/release.yml) and published as a
[GitHub Release](https://github.com/daoek/Pinfit/releases) with the JAR and a `SHA256SUMS`
checksum file attached.

=== "Windows"

    In PowerShell, one line installs the newest release:

    ```powershell
    irm https://raw.githubusercontent.com/daoek/Pinfit/main/scripts/install.ps1 | iex
    ```

    To pin a specific version, download
    [`scripts/install.ps1`](https://github.com/daoek/Pinfit/raw/main/scripts/install.ps1) and run it
    with that version (`latest` also works):

    ```powershell
    .\install.ps1 -Version 0.1.0-beta.5
    ```

    !!! tip "If PowerShell blocks the downloaded script"

        When your machine's default execution policy refuses to run a local `.ps1`, invoke it
        through an explicitly scoped bypass instead:

        ```powershell
        powershell -ExecutionPolicy Bypass -File .\install.ps1 -Version 0.1.0-beta.5
        ```

        That flag applies only to the single `powershell.exe` invocation it is passed to. It does
        not change the execution policy for your machine or your user account.

=== "Linux / macOS"

    One line installs the newest release:

    ```console
    curl -fsSL https://raw.githubusercontent.com/daoek/Pinfit/main/scripts/install.sh | sh
    ```

    To pin a specific version, pass it through to the script:

    ```console
    curl -fsSL https://raw.githubusercontent.com/daoek/Pinfit/main/scripts/install.sh | sh -s -- --version 0.1.0-beta.5
    ```

"Newest" means the newest stable release, or - while only prereleases exist - the newest
prerelease. The one-liners fetch the installer from `main`; if you would rather read it before
running it, download it first and run the file instead.

The script picks the bundle for your OS and CPU (`pinfit-<version>-windows-x64.zip`,
`-linux-x64.tar.gz`, `-linux-aarch64.tar.gz`, `-macos-x64.tar.gz` or `-macos-aarch64.tar.gz`),
downloads it and `SHA256SUMS` over HTTPS from GitHub, verifies its SHA-256 against the published
checksum, and installs it **only** if the checksum matches. Nothing is written to disk otherwise.
Re-running it upgrades in place: the old installation, runtime included, is replaced as a whole. At
the end it prints the installed JAR's checksum.

Releases published before the bundled runtime was introduced (up to `0.1.0-beta.4`) have no bundle
and cannot be installed with this script.

## Build and install from source

For contributors, or to install an unreleased build. Requires a **JDK 17 or newer** (with `jlink`,
which every standard JDK has) and **Maven** — the one case where Java is needed, since it compiles
Pinfit and builds the runtime that gets bundled.

```console
git clone https://github.com/daoek/Pinfit.git
cd Pinfit
mvn clean package
```

That produces `target/pinfit-<version>.jar` (the version comes from `pom.xml`), which already contains SnakeYAML and can be copied
anywhere without a separate dependency directory. You can run it directly:

```console
java -jar target/pinfit-*.jar --help
```

To install it as a `pinfit` command instead, run the same installer from the repository root with
no version:

=== "Windows"

    ```powershell
    .\scripts\install.ps1
    ```

=== "Linux / macOS"

    ```console
    ./scripts/install.sh
    ```

In this mode the script builds the bundle locally (`mvn -Pbundle clean package`: the jar, a
`jlink` runtime from your JDK, and the launcher, in `target/bundle`) rather than downloading
anything, then installs that.

!!! tip "VS Code task"

    Press ++ctrl+shift+b++ and run the default **Pinfit: Package + Install** task to rebuild, test,
    package and update the installed command in one step (Windows).

## What the installer touches

Both modes, on both platforms, behave the same way:

- Runs entirely as your user — no administrator/`sudo` rights needed.
- Is marker-gated: it refuses to overwrite a directory it did not create itself, and the matching
  uninstall script refuses to remove anything without that same marker present.
- Writes only inside the install directory itself:

=== "Windows"

    `%LOCALAPPDATA%\Pinfit` by default, or wherever `-InstallDirectory` points — and adds that one
    directory to your **user** `PATH` (via the registry, so no file is edited for this).

=== "Linux / macOS"

    `$HOME/.local/share/Pinfit` by default, or wherever `--install-dir` points. Unlike the Windows
    registry, editing a shell profile to add to `PATH` means picking a file among several
    (`~/.bashrc`, `~/.zshrc`, `~/.profile`, ...) and guessing which one you actually use — this
    script does not do that for you. If the install directory isn't already on `PATH`, it prints
    the one line to add yourself:

    ```console
    export PATH="$HOME/.local/share/Pinfit:$PATH"
    ```

The installed copy is independent from `target/`, so `mvn clean` will not remove it. Re-run the
installer in either mode to update it.

## Verify

Open a **new** terminal (or, on Linux/macOS, source your profile) so the updated `PATH` is picked up:

```console
pinfit --help
```

```title="Expected output"
Pinfit - YAML-driven C interface and module generator

Usage: pinfit <command> [options]

Commands:
  init             Create a new pinfit.yaml project
  create           Scaffold a new interface, module, or other spec
  ...
```

See the [CLI reference](../reference/cli.md) for the full command list.

## Uninstall

=== "Windows"

    ```powershell
    & "$env:LOCALAPPDATA\Pinfit\Uninstall-Pinfit.ps1"
    ```

=== "Linux / macOS"

    `install.sh` copies its own `uninstall.sh` into the install directory, so:

    ```console
    ~/.local/share/Pinfit/uninstall.sh
    ```

This removes the install directory (and, on Windows, its `PATH` entry). It does not touch any
project: your `pinfit.yaml` files and generated C code stay exactly as they are. To remove Pinfit from
a *project*, use [`pinfit detach`](../reference/cli.md#pinfit-detach).

## Why a bundled runtime, not a single native binary

A GraalVM native-image binary would be smaller still, but SnakeYAML's YAML parsing leans on
reflection in places `native-image` needs explicit reachability metadata for. A `jlink` runtime
runs the exact jar the tests run, with no such risk. It contains only the two JDK modules Pinfit
uses (`java.base` and `java.logging`).

## Running without installing

Every release also publishes the plain `pinfit-<version>.jar`. If you already have Java 17 or newer,
you can run that directly instead of installing — substitute it for `pinfit` in every command:

```console
java -jar path/to/pinfit-<version>.jar generate
```
