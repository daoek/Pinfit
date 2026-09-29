#!/usr/bin/env sh
# Compiles every golden expected output (src/test/resources/golden/*/expected) as C99 with
# warnings as errors, so a generator change that emits non-compiling or warning-prone C fails CI.
# Headers are also compiled on their own, which catches a header missing its own includes.
#
# -Wno-unused-function / -Wno-unused-variable: a freshly generated module keeps private variables
# and static functions that only the user's region code will reference.
#
# Usage: scripts/ci/compile-golden.sh            (uses $CC, default gcc)
#        CC=arm-none-eabi-gcc scripts/ci/compile-golden.sh
set -eu

CC="${CC:-gcc}"
FLAGS="-std=c99 -Wall -Wextra -Wpedantic -Werror -Wno-unused-function -Wno-unused-variable"
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
WORK="$(mktemp -d "${TMPDIR:-/tmp}/pinfit-compile.XXXXXX")"
trap 'rm -rf "$WORK"' EXIT

failures=0
count=0
for expected in "$ROOT"/src/test/resources/golden/*/expected; do
    for file in $(find "$expected" -name '*.c' -o -name '*.h' | sort); do
        count=$((count + 1))
        case "$file" in
            *.c) unit="$file" ;;
            *.h) unit="$WORK/unit.c"; printf '#include "%s"\n' "$(basename "$file")" > "$unit" ;;
        esac
        # shellcheck disable=SC2086
        if ! "$CC" $FLAGS -I "$(dirname "$file")" -c "$unit" -o "$WORK/out.o"; then
            echo "FAILED: $file" >&2
            failures=$((failures + 1))
        fi
    done
done

echo "Compiled $count generated file(s) with $CC, $failures failure(s)."
[ "$failures" -eq 0 ]
