#!/bin/bash
# Assemble cc1's output with GNU as the way SN's asn64 would.
#
#   tools/sn_as.sh <in.s> <out.o>
#
# asn64 is the original assembler, but it only runs under wine, and inside
# the emulated amd64 container that costs ~30 s a file against well under one
# for gas. Matching the same sources through both showed two differences, and
# both are reproduced here:
#
#  - asn64 spells `move` as `addu rd, rs, $0`; gas uses `or`.
#  - asn64 expands `li.d $fN, value` by putting the double in .rodata and
#    loading it with lui/ldc1, where gas builds it in registers with
#    lui/mtc1. The ROM has the rodata form (func_00271748 reads 2^32 from
#    D_800A80D8), so li.d becomes a literal plus the load here too.
#  - asn64 fills the c.cond -> bc1t/bc1f hazard with a nop, and no other
#    hazard (not the load delay, not mtc1). cc1 marks every hazard it leaves
#    to the assembler with a `#nop` comment, so turning exactly the one after
#    a compare into a real nop reproduces asn64, while -march=vr4300 keeps
#    gas from adding any of its own.
#
# `make verify` is the arbiter: if gas ever diverges from asn64 the SHA1
# check fails, and `make ASSEMBLER=asn64` builds with the original tool.
set -euo pipefail
IN=$1
OUT=$2
INC_DIR=${INC_DIR:-us/include}

tr -d '\r' < "$IN" \
    | sed -E 's/^([[:space:]]+)move([[:space:]]+)([^,]+),([^,#]+)/\1addu\2\3,\4,$0/' \
    | awk '{ if (prev_cmp && $0 ~ /^[[:space:]]*#nop[[:space:]]*$/) { print "\tnop"; prev_cmp = 0; next }
             prev_cmp = ($1 ~ /^c\.[a-z]+\.[sd]$/); print }' \
    | awk '$1 == "li.d" { split($2, a, ","); n++
                          printf "\t.section .rodata\n\t.align 3\n.Lsn_lit%d:\n\t.double %s\n\t.text\n", n, a[2]
                          printf "\tlui $at,%%hi(.Lsn_lit%d)\n\tldc1 %s,%%lo(.Lsn_lit%d)($at)\n", n, a[1], n
                          next }
           { print }' \
    | mips-linux-gnu-as -EB -mabi=32 -march=vr4300 -mtune=vr4300 -mips3 -O1 \
        --no-pad-sections -I "$INC_DIR" -o "$OUT" -
