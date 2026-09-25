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
#  - asn64 expands `li.d`/`li.s` differently from gas: into a float
#    register it puts the constant in .rodata and loads it (the ROM's form,
#    e.g. func_0027BAEC's pool); into an integer register (a float argument
#    in $a1) it loads the float's bits as an integer. tools/sn_li.py.
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
    | python3 "$(dirname "$0")/sn_li.py" \
    | mips-linux-gnu-as -EB -mabi=32 -march=vr4300 -mtune=vr4300 -mips3 -O1 \
        --no-pad-sections -I "$INC_DIR" -o "$OUT" -
