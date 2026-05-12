#!/usr/bin/env bash
# Pulls third-party material into the working tree from local clones.
# Nothing this script copies is committed — it's all under .gitignore.
#
# Required clones (see README):
#   references/turok3/      ← https://github.com/Drahsid/turok3
#   references/LibTEngine/  ← https://github.com/Drahsid/LibTEngine
#
# Re-run any time the references update.

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
REFS="$ROOT/references"

require_dir() {
    if [ ! -d "$1" ]; then
        echo "error: missing $1"
        echo "       clone the reference repo first (see README)"
        exit 1
    fi
}

require_dir "$REFS/turok3"
require_dir "$REFS/LibTEngine"

echo ">> SN64 toolchain → tools/sn64/"
mkdir -p "$ROOT/tools/sn64"
cp -R "$REFS/turok3/tools/mips-gcc/sn64/." "$ROOT/tools/sn64/"

echo ">> psyq-obj-parser binary → tools/psyq-obj-parser"
cp "$REFS/turok3/tools/psyq-obj-parser" "$ROOT/tools/psyq-obj-parser"
chmod +x "$ROOT/tools/psyq-obj-parser"

echo ">> LibTEngine T2 struct headers → include/types/"
mkdir -p "$ROOT/include/types"
cp -R "$REFS/LibTEngine/lib/libtengine/lib/types/." "$ROOT/include/types/"

echo ">> LibTEngine T2 release header → include/libtengine/"
mkdir -p "$ROOT/include/libtengine"
cp "$REFS/LibTEngine/lib/libtengine/turok2/release_us.h" \
   "$ROOT/include/libtengine/release_us.h"

echo ">> functions.csv → versions/ (untracked, used to seed symbols)"
cp "$REFS/LibTEngine/functions.csv" "$ROOT/versions/functions.csv.libtengine"

echo ">> generating versions/symbol_addrs.us.txt from functions.csv"
# LibTEngine repeats names (compiler-emitted trampolines etc). Splat wants
# unique symbol names per address — disambiguate by appending the addr.
awk -F'","' 'NR>1 {
    gsub(/"/,""); n=split($0,a,","); name=a[1]; loc=a[2];
    if (loc !~ /^800/) next;
    seen[name]++;
    sym = (seen[name] == 1) ? name : name "_" loc;
    print sym " = 0x" loc "; // type:func"
}' "$ROOT/versions/functions.csv.libtengine" > "$ROOT/versions/symbol_addrs.us.txt"
echo "   $(wc -l < "$ROOT/versions/symbol_addrs.us.txt") T2 symbols seeded"

echo "done."
