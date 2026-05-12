#!/usr/bin/env bash
# Pulls third-party material into the working tree from local clones.
# Nothing this script copies is committed — it's all under .gitignore.
#
# Required clones (see README):
#   references/turok3/      ← https://github.com/Drahsid/turok3
#   references/LibTEngine/  ← https://github.com/Drahsid/LibTEngine
#
# Re-run any time the references update. Idempotent.

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
REFS="$ROOT/references"

# ---------------------------------------------------------------------------
# Worktree bootstrap: if we're running inside .claude/worktrees/agent-*,
# symlink the heavy artifacts (baserom, references/) from the main checkout
# at ../../../ so each worktree doesn't need its own copy. Skip if the
# target already exists (idempotent).
# ---------------------------------------------------------------------------
case "$ROOT" in
    */.claude/worktrees/agent-*)
        MAIN_ROOT="$(cd "$ROOT/../../.." && pwd)"
        echo ">> worktree detected — bootstrapping from $MAIN_ROOT"

        if [ ! -e "$ROOT/baserom.us.z64" ] && [ -f "$MAIN_ROOT/baserom.us.z64" ]; then
            echo "   symlink baserom.us.z64"
            ln -s "$MAIN_ROOT/baserom.us.z64" "$ROOT/baserom.us.z64"
        fi

        if [ ! -e "$ROOT/references" ] && [ -d "$MAIN_ROOT/references" ]; then
            echo "   symlink references/"
            ln -s "$MAIN_ROOT/references" "$ROOT/references"
        fi
        ;;
esac

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
# Make sure native binaries are executable (cp may strip the +x bit when
# the source lives on a filesystem without exec perms, e.g. some FUSE
# mounts). This was a Round 3 gap.
chmod +x "$ROOT/tools/sn64/"* 2>/dev/null || true

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
#
# T3-STALE FILTER (Agent K, Pass 4 — docs/SEGMENTS.md § 4.1):
# The LibTEngine CSV is harvested from Turok 3, whose libultra is laid
# out at higher VRAMs than T2's. T2's libultra .text ends at 0xDEDE0
# (vram 0x800DEDE0). Any entry at or above that whose name begins with
# `os` or `__os` is a Turok 3 address — drop it so it doesn't confuse
# splat/m2c about T2's actual libultra layout. The hand-verified T2
# libultra names are appended further below.
awk -F'","' 'NR>1 {
    gsub(/"/,""); n=split($0,a,","); name=a[1]; loc=a[2];
    if (loc !~ /^800/) next;
    # T3-stale libultra filter: drop os* / __os* at >= 0x800DEDE0
    locu = toupper(loc);
    if ((name ~ /^os/ || name ~ /^__os/) && locu >= "800DEDE0") next;
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
    # NOTE: addresses below were re-verified after the splat libultra
    # vram fix (0x800C2474). Cross-checked against direct byte search
    # for mfc0/mtc0 $12 + jr ra patterns. Previous values in this list
    # were 0x400 low (assumed wrong segment vram of 0x800D6BE0 base).
    echo "__osGetSR            = 0x800D6FE0; // type:func"
    echo "__osDisableInt       = 0x800D6FF0; // type:func"
    echo "__osRestoreInt       = 0x800D7010; // type:func"
    echo "osInvalDCache        = 0x800D702C; // type:func"
    echo "osWritebackDCache    = 0x800D70D0; // type:func"
    echo "osVirtualToPhysical  = 0x800D7140; // type:func"
    echo "__osSetCompare       = 0x800D7210; // type:func"
    echo "__osSetFpcCsr        = 0x800D7220; // type:func"
    echo "osSetIntMask         = 0x800D7230; // type:func"
    echo "__osSetSR            = 0x800D72E0; // type:func"
    echo "__osInitialize_TLB   = 0x800D72F0; // type:func"
    echo "osInvalICache        = 0x800D7340; // type:func"
    echo "osWritebackDCacheAll = 0x800D73B0; // type:func"
} >> "$ROOT/versions/symbol_addrs.us.txt"
echo "   17 libultra names appended"

# -- Pass 5 (Agent O): main entry alias ----------------------------------
# The SN64 entry stub at ROM 0x1000 jumps to `main` at VRAM 0x8000aa50.
# The LibTEngine CSV already contains a duplicate-disambiguated `main_8000aa50`
# entry, so we rewrite it in-place to the canonical name `main` (the awk
# pass above appends `_<addr>` when it sees the name twice; we only want
# the first occurrence to win as the real entrypoint).
if grep -q '^main_8000aa50 = 0x8000AA50;' "$ROOT/versions/symbol_addrs.us.txt" \
 || grep -q '^main_8000aa50 = 0x8000aa50;' "$ROOT/versions/symbol_addrs.us.txt"; then
    sed -i.bak -E 's/^main_8000aa50 = (0x[0-9A-Fa-f]+);/main = \1;/' \
        "$ROOT/versions/symbol_addrs.us.txt"
    rm -f "$ROOT/versions/symbol_addrs.us.txt.bak"
    echo "   main alias renamed main_8000aa50 -> main"
else
    echo "main = 0x8000aa50; // type:func" >> "$ROOT/versions/symbol_addrs.us.txt"
    echo "   main alias appended (0x8000aa50)"
fi

# -- LNK decoder integration (Agent J) -----------------------------------
# tools/lnk_decoder.py extracts symbol names embedded in the SN64 LNK
# debug records around ROM offset 0x107000. The addresses are
# section-relative (small offsets like 0x160, not VRAM), so they're not
# usable as splat seeds directly — but the *names* (InitPTimers,
# UpdatePTimers, DoInit, DoUpdate, UpdateProfileTimer, VM_PhysicalPool)
# are real T2 symbols that downstream tooling can pick up once a real
# linker pass assigns VRAMs.
#
# We append the decoder output verbatim, deduped by symbol name against
# what's already in the seed. If the decoder fails (missing baserom,
# python missing, etc.) we warn and continue.
LNK_PY="$ROOT/tools/lnk_decoder.py"
BASEROM="$ROOT/baserom.us.z64"
if [ -f "$LNK_PY" ] && [ -f "$BASEROM" ]; then
    echo ">> running lnk_decoder.py and appending to symbol seed"
    LNK_OUT="$(mktemp)"
    if python3 "$LNK_PY" --rom "$BASEROM" --symbols > "$LNK_OUT" 2>/dev/null; then
        # Dedupe: collect existing symbol names from the seed, then emit
        # only LNK lines whose name is new. Pass-through comment lines.
        awk -v seed="$ROOT/versions/symbol_addrs.us.txt" '
            BEGIN {
                while ((getline line < seed) > 0) {
                    if (line ~ /^[[:space:]]*[A-Za-z_][A-Za-z0-9_]*[[:space:]]*=/) {
                        n = line
                        sub(/[[:space:]]*=.*/, "", n)
                        gsub(/^[[:space:]]+|[[:space:]]+$/, "", n)
                        seen[n] = 1
                    }
                }
                close(seed)
                print ""
                print "// --- symbols from tools/lnk_decoder.py (section-relative offsets) ---"
            }
            /^[[:space:]]*[A-Za-z_][A-Za-z0-9_]*[[:space:]]*=/ {
                # extract symbol name
                name = $0
                sub(/[[:space:]]*=.*/, "", name)
                gsub(/^[[:space:]]+|[[:space:]]+$/, "", name)
                if (name in seen) next
                seen[name] = 1
                # LNK offsets are section-relative (tiny, collide with each
                # other and with VRAMs), so comment them out: keep the name
                # discoverable but do not feed it to splat as a live symbol.
                print "// " $0
                next
            }
            { print }
        ' "$LNK_OUT" >> "$ROOT/versions/symbol_addrs.us.txt"
        ADDED="$(grep -cE '^[[:space:]]*[A-Za-z_][A-Za-z0-9_]*[[:space:]]*=' "$LNK_OUT" || true)"
        echo "   ${ADDED:-0} LNK symbol candidates considered (deduped against seed)"
    else
        echo "   warning: lnk_decoder.py failed, skipping LNK integration" >&2
    fi
    rm -f "$LNK_OUT"
else
    echo "   warning: skipping LNK integration (missing $LNK_PY or $BASEROM)" >&2
fi

echo "done."
