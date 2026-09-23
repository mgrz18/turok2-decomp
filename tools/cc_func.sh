#!/bin/bash
# Compile one C file through the SN64 pipeline and leave an ELF object.
#
#   cpp -> cc1 (SN64 GCC) -> asn64 (wine) -> psyq-obj-parser -> ELF .o
#
# Runs inside the turok2-build container; `tools/match_func.py` drives it.
# Same stages and flags as the Makefile's C rules, but for a single file and
# with the compiler selectable, so candidate compilers can be compared on the
# same source without touching the build.
#
#   tools/cc_func.sh <in.c> <out.o> [cc1 name in tools/sn64] [opt flags]
set -euo pipefail

IN=$1
OUT=$2
CC_NAME=${3:-cc1}
OPT=${4:--O2}

TOOLS=/work/tools
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

case "$CC_NAME" in
    *.exe) CC="wine $TOOLS/sn64/$CC_NAME" ;;
    *)     CC="$TOOLS/sn64/$CC_NAME" ;;
esac

cpp -P -undef -Wall -lang-c -D_LANGUAGE_C -DF3DEX_GBI_2 -D__GNUC__=2 \
    -I/work/include -I/work/us/include -nostdinc -o "$WORK/f.i" "$IN"
unix2dos -q "$WORK/f.i"
$CC -quiet -G0 -mips3 $OPT -mgas -meb -mcpu=VR4300 -mhard-float -mfp64 \
    -o "$WORK/f.s" "$WORK/f.i" 2>"$WORK/cc.log" || { cat "$WORK/cc.log" >&2; exit 1; }
# asn64 refuses the GNU directives. The Makefile also prepends `.set noat` to
# quiet $at warnings, but that turns a float load from a symbol (`l.s $f1,
# D_800A7948`, which needs $at for the %hi) into a hard error. Leave $at alone
# and judge asn64 by whether it wrote the object, not by its exit code.
sed -i -e 's/\.version/#.version/g' -e 's/\.size/#.size/g' \
    -e 's/\.type/#.type/g' "$WORK/f.s"
unix2dos -q "$WORK/f.s"
cp "$WORK/f.s" "${OUT%.o}.s"
( cd "$WORK" && wine "$TOOLS/sn64/asn64.exe" -I /work/include -mips3 -o f.obj f.s ) >"$WORK/as.log" 2>&1 || true
[ -s "$WORK/f.obj" ] || { cat "$WORK/as.log" >&2; exit 1; }
"$TOOLS/psyq-obj-parser" "$WORK/f.obj" -o "$OUT" -b -n >/dev/null
