#!/usr/bin/env sh
#
# Installs Pinfit into a per-user directory (default $HOME/.local/share/Pinfit) on Linux or macOS.
# No root/sudo is used, and - unlike a PATH change on Windows, which install.ps1 makes through
# the registry - this script never edits your shell profile: it prints the one line to add
# yourself, so nothing outside the install directory is ever touched.
#
# One-line install of the newest release:
#
#     curl -fsSL https://raw.githubusercontent.com/daoek/Pinfit/main/scripts/install.sh | sh
#
# What gets installed is self-contained: pinfit.jar plus its own trimmed Java runtime and a
# launcher that only uses that runtime. No Java has to be installed on the machine.
#
# Two modes:
#   - Pass --version <tag> (e.g. "0.1.0"), or --version latest, to download that release's
#     bundle for this OS and CPU (linux-x64, linux-aarch64, macos-x64, macos-aarch64) and
#     SHA256SUMS from GitHub over HTTPS, verify the checksum, and install only if it matches.
#     Nothing is written to the install directory if verification fails. This is also the default
#     whenever the script is not run from a repository checkout (e.g. piped into sh).
#   - Run from a checkout without --version to build the bundle instead
#     (`mvn -Pbundle clean package`, which needs a JDK 17+ with jlink), which is what contributors
#     use.
set -eu

REPO_SLUG="daoek/Pinfit"
INSTALL_DIR="${PINFIT_INSTALL_DIR:-$HOME/.local/share/Pinfit}"
VERSION=""
SKIP_BUILD=0
MARKER_NAME=".pinfit-install-marker"

usage() {
    echo "Usage: install.sh [--version <tag>|latest] [--install-dir <dir>] [--skip-build]" >&2
    exit 1
}

while [ $# -gt 0 ]; do
    case "$1" in
        --version)
            [ $# -ge 2 ] || usage
            VERSION="$2"
            shift 2
            ;;
        --install-dir)
            [ $# -ge 2 ] || usage
            INSTALL_DIR="$2"
            shift 2
            ;;
        --skip-build)
            SKIP_BUILD=1
            shift
            ;;
        -h|--help)
            usage
            ;;
        *)
            usage
            ;;
    esac
done

if [ -n "$VERSION" ] && [ "$SKIP_BUILD" -eq 1 ]; then
    echo "--version and --skip-build are mutually exclusive: --version installs a downloaded release and never builds locally." >&2
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Piped into sh, $0 is the shell itself, so there is no checkout next to "this script" to build.
if [ -z "$VERSION" ] && { [ ! -f "$SCRIPT_DIR/pinfit.sh" ] || [ ! -f "$REPO_ROOT/pom.xml" ]; }; then
    if [ "$SKIP_BUILD" -eq 1 ]; then
        echo "--skip-build needs a repository checkout; run install.sh from inside one." >&2
        exit 1
    fi
    VERSION="latest"
fi

# Refuse the handful of directories that would make "rm -rf $INSTALL_DIR" (in uninstall.sh)
# catastrophic if a user pointed --install-dir somewhere careless.
case "$INSTALL_DIR" in
    "$HOME"|"$HOME"/|/|"")
        echo "Refusing unsafe installation directory: $INSTALL_DIR" >&2
        exit 1
        ;;
esac

if [ -d "$INSTALL_DIR" ] && [ -n "$(ls -A "$INSTALL_DIR" 2>/dev/null)" ] && [ ! -f "$INSTALL_DIR/$MARKER_NAME" ]; then
    echo "Refusing to overwrite non-Pinfit directory: $INSTALL_DIR" >&2
    exit 1
fi

sha256_of() {
    if command -v sha256sum >/dev/null 2>&1; then
        sha256sum "$1" | awk '{print $1}'
    elif command -v shasum >/dev/null 2>&1; then
        shasum -a 256 "$1" | awk '{print $1}'
    else
        echo "Neither sha256sum nor shasum is available - cannot verify the download." >&2
        exit 1
    fi
}

# Verifies $1's SHA-256 against the entry for $2 in the SHA256SUMS file at $3 (the standard
# "<hash>  <filename>" or "<hash> *<filename>" format). Exits - installing nothing - on any
# mismatch or missing entry.
assert_sha256_match() {
    file_path=$1
    file_name=$2
    sums_path=$3
    expected=$(awk -v name="$file_name" '$2 == name || $2 == "*" name {print $1; exit}' "$sums_path")
    if [ -z "$expected" ]; then
        echo "No checksum entry for '$file_name' found in $sums_path" >&2
        exit 1
    fi
    actual=$(sha256_of "$file_path")
    if [ "$actual" != "$expected" ]; then
        echo "Checksum mismatch for '$file_name': expected $expected, got $actual. Refusing to install." >&2
        exit 1
    fi
    echo "Checksum verified for $file_name ($actual)"
}

# Newest stable release; before the first stable one exists, the newest prerelease.
resolve_latest_tag() {
    # github.com/.../releases/latest redirects to the newest stable release's tag page. Unlike the
    # REST API it is not rate-limited (60 calls/hour per IP, easily hit behind a shared office IP).
    location=$(curl -fsSI -o /dev/null -w '%{redirect_url}' "https://github.com/$REPO_SLUG/releases/latest" 2>/dev/null || true)
    case "$location" in
        */releases/tag/*) echo "${location##*/}"; return 0 ;;
    esac
    # No stable release yet: the API lists prereleases too.
    json=$(curl -fsSL "https://api.github.com/repos/$REPO_SLUG/releases?per_page=1") || return 1
    printf '%s\n' "$json" | sed -n 's/.*"tag_name": *"\([^"]*\)".*/\1/p' | head -n 1
}

if [ "$VERSION" = "latest" ]; then
    VERSION=$(resolve_latest_tag) || VERSION=""
    if [ -z "$VERSION" ]; then
        echo "Could not look up the latest release at https://github.com/$REPO_SLUG/releases" >&2
        exit 1
    fi
    echo "Latest release: $VERSION"
fi

# The release's bundle name for this machine, e.g. linux-x64 or macos-aarch64.
detect_platform() {
    case "$(uname -s)" in
        Linux) os=linux ;;
        Darwin) os=macos ;;
        *) echo "Unsupported operating system '$(uname -s)': Pinfit bundles exist for Linux and macOS (and Windows via install.ps1)." >&2; return 1 ;;
    esac
    case "$(uname -m)" in
        x86_64|amd64) arch=x64 ;;
        aarch64|arm64) arch=aarch64 ;;
        *) echo "Unsupported CPU '$(uname -m)': Pinfit bundles exist for x64 and aarch64." >&2; return 1 ;;
    esac
    echo "$os-$arch"
}

if [ -n "$VERSION" ]; then
    case "$VERSION" in
        v*) tag="$VERSION" ;;
        *) tag="v$VERSION" ;;
    esac
    bare_version=${tag#v}
    platform=$(detect_platform)
    bundle_name="pinfit-$bare_version-$platform.tar.gz"
    release_base_url="https://github.com/$REPO_SLUG/releases/download/$tag"

    download_dir=$(mktemp -d "${TMPDIR:-/tmp}/pinfit-install.XXXXXX")
    trap 'rm -rf "$download_dir"' EXIT

    echo "Downloading $bundle_name from release $tag..."
    if ! curl -fsSL -o "$download_dir/$bundle_name" "$release_base_url/$bundle_name" \
            || ! curl -fsSL -o "$download_dir/SHA256SUMS" "$release_base_url/SHA256SUMS"; then
        echo "Could not download $bundle_name from release '$tag' (https://github.com/$REPO_SLUG/releases)." >&2
        echo "Releases before the bundled Java runtime was introduced have no such file - install a newer version." >&2
        exit 1
    fi
    assert_sha256_match "$download_dir/$bundle_name" "$bundle_name" "$download_dir/SHA256SUMS"
    bundle_dir="$download_dir/bundle"
    mkdir -p "$bundle_dir"
    tar -xzf "$download_dir/$bundle_name" -C "$bundle_dir"
else
    if [ "$SKIP_BUILD" -eq 0 ]; then
        if ! command -v mvn >/dev/null 2>&1; then
            echo "Maven (mvn) was not found on PATH." >&2
            exit 1
        fi
        # -Pbundle adds the jlink runtime and launcher (target/bundle). Clean first so Shade never
        # consumes a JAR that was already shaded by a prior build.
        (cd "$REPO_ROOT" && mvn -Pbundle clean package)
    fi
    bundle_dir="$REPO_ROOT/target/bundle"
fi

for required in pinfit.jar pinfit runtime/bin/java; do
    if [ ! -f "$bundle_dir/$required" ]; then
        echo "The Pinfit bundle in $bundle_dir is incomplete (no $required). Build it with 'mvn -Pbundle package', or run without --skip-build." >&2
        exit 1
    fi
done

# Replace the whole installation, runtime included, so no file from an older version lingers. The
# marker check above already proved this directory is Pinfit's own.
mkdir -p "$INSTALL_DIR"
find "$INSTALL_DIR" -mindepth 1 -maxdepth 1 ! -name "$MARKER_NAME" -exec rm -rf {} +
cp -R "$bundle_dir"/. "$INSTALL_DIR"/
chmod +x "$INSTALL_DIR/pinfit" "$INSTALL_DIR/uninstall.sh" "$INSTALL_DIR/runtime/bin/"*
echo "Pinfit managed installation. Safe removal requires this marker." > "$INSTALL_DIR/$MARKER_NAME"

installed_hash=$(sha256_of "$INSTALL_DIR/pinfit.jar")
echo "Pinfit installed in $INSTALL_DIR"
echo "Installed jar SHA256: $installed_hash"
echo "To uninstall later: $INSTALL_DIR/uninstall.sh"

case ":$PATH:" in
    *":$INSTALL_DIR:"*) ;;
    *)
        echo
        echo "Add $INSTALL_DIR to your PATH - add this line to your shell profile (~/.bashrc,"
        echo "~/.zshrc, ~/.profile, ...) and open a new terminal:"
        echo
        echo "    export PATH=\"$INSTALL_DIR:\$PATH\""
        echo
        echo "This script does not edit that file for you - nothing outside $INSTALL_DIR is touched."
        ;;
esac
echo "Then run: pinfit --help"
