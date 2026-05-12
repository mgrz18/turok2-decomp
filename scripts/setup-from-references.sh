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

# -- libultra overrides --------------------------------------------------
# The LibTEngine CSV is sourced from Turok 3 and its libultra addresses
# do NOT match Turok 2's layout. The names below were identified by
# direct disassembly of the T2 libultra segment (ROM 0xC3000..0xDC000)
# using COP0/cache prologue signatures. Addresses use the corrected
# segment VRAM delta of 0x7FFFF400 (see docs/SEGMENTS.md and
# docs/LIBULTRA-MATCHING.md).
#
# Appending these AFTER the awk pass intentionally lets splat resolve
# them later in the file — splat takes the last definition for a given
# VRAM. Any T3-stale libultra entries from the CSV are therefore
# shadowed by these.
{
    echo ""
    echo "// --- T2 libultra (identified via prologue/COP0 disasm) ---"
    echo "osMapTLB             = 0x800C7E60; // type:func"
    echo "osUnmapTLB           = 0x800C7EC0; // type:func"
    echo "osGetTLB             = 0x800C7EFC; // type:func"
    echo "__osCleanupThread    = 0x800C86D8; // type:func"
    echo "__osGetSR            = 0x800D6BE0; // type:func"
    echo "__osDisableInt       = 0x800D6BF0; // type:func"
    echo "__osRestoreInt       = 0x800D6C10; // type:func"
    echo "osInvalDCache        = 0x800D6C2C; // type:func"
    echo "osWritebackDCache    = 0x800D6CD0; // type:func"
    echo "osVirtualToPhysical  = 0x800D6D40; // type:func"
    echo "__osSetCompare       = 0x800D6E10; // type:func"
    echo "__osSetFpcCsr        = 0x800D6E20; // type:func"
    echo "osSetIntMask         = 0x800D6E30; // type:func"
    echo "__osSetSR            = 0x800D6EE0; // type:func"
    echo "__osInitialize_TLB   = 0x800D6EF0; // type:func"
    echo "osInvalICache        = 0x800D6F40; // type:func"
    echo "osWritebackDCacheAll = 0x800D6FB0; // type:func"
} >> "$ROOT/versions/symbol_addrs.us.txt"
echo "   17 libultra names appended"

echo "done."
