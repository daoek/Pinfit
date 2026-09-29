#!/usr/bin/env sh
# Launcher installed as "$INSTALL_DIR/pinfit". Runs pinfit.jar on the Java runtime bundled next to
# it (runtime/, made by jlink at release time) - never on a system Java, so nothing else needs to
# be installed and a JAVA_HOME or PATH change can't break it.

# Follow symlinks to the real launcher, so a link to it from ~/bin or /usr/local/bin still finds
# runtime/ and pinfit.jar next to the target rather than next to the link.
SELF="$0"
while [ -h "$SELF" ]; do
    LINK=$(readlink "$SELF")
    case "$LINK" in
        /*) SELF="$LINK" ;;
        *) SELF="$(dirname "$SELF")/$LINK" ;;
    esac
done
DIR="$(cd "$(dirname "$SELF")" && pwd)"

if [ ! -x "$DIR/runtime/bin/java" ] || [ ! -f "$DIR/pinfit.jar" ]; then
    echo "Pinfit's bundled runtime is missing or incomplete in $DIR - reinstall Pinfit:" >&2
    echo "  curl -fsSL https://raw.githubusercontent.com/daoek/Pinfit/main/scripts/install.sh | sh" >&2
    exit 1
fi

exec "$DIR/runtime/bin/java" -jar "$DIR/pinfit.jar" "$@"
