#!/usr/bin/env bash
# scripts/verify_boot_asm.sh
#
# Assemble src/us/asm/boot.s standalone (inside the turok2-build
# linux/amd64 docker image) and diff its .text against bytes
# 0x1000..0x10FC of baserom.us.z64.
#
# NOTE: boot.s is now wired into the main build via `hasm` in
# versions/turok2.us.yaml (subsegment ../src/us/asm/boot), so
# `make verify` is the canonical end-to-end check once the full ROM
# links. This script remains useful as a focused sanity check that
# isolates the boot.s assembly from the rest of the pipeline — handy
# when iterating on the boot routine itself.
#
# Usage:
#   ./scripts/verify_boot_asm.sh           # run via docker
#   IN_CONTAINER=1 ./scripts/verify_boot_asm.sh   # already inside container
#
# Exits 0 on byte-exact match, 1 otherwise.

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

if [[ "${IN_CONTAINER:-0}" != "1" ]]; then
    exec docker run --platform=linux/amd64 --rm \
        -e IN_CONTAINER=1 \
        -v "$ROOT":/work -w /work \
        turok2-build bash scripts/verify_boot_asm.sh
fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

# Stub out external symbols so the assembler can resolve them. Values
# chosen to match the original linker output (main lives at 0x0028D380
# in the real ROM, STVM_MapTLB is defined in this same file).
cat >"$WORK/stubs.s" <<'EOF'
.set main, 0x0028D380
EOF

cat "$WORK/stubs.s" src/us/asm/boot.s >"$WORK/boot.s"

mips-linux-gnu-as -EB -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -O1 \
    -I us/include --defsym ASSEMBLER=1 \
    -o "$WORK/boot.o" "$WORK/boot.s"

# Link at VRAM 0x80000400 so jal STVM_MapTLB resolves to the correct
# 26-bit target (and any other intra-section references settle).
cat >"$WORK/boot.ld" <<'EOF'
SECTIONS {
    . = 0x80000400;
    .text : { *(.text) }
    /DISCARD/ : { *(*) }
}
EOF

mips-linux-gnu-ld -T "$WORK/boot.ld" --no-check-sections \
    -o "$WORK/boot.elf" "$WORK/boot.o"

mips-linux-gnu-objcopy -O binary -j .text "$WORK/boot.elf" "$WORK/boot.bin"

# Reference: bytes 0x1000..0x10FC (256 bytes) of baserom
dd if=baserom.us.z64 of="$WORK/ref.bin" bs=1 skip=4096 count=256 \
    status=none

if cmp -s "$WORK/boot.bin" "$WORK/ref.bin"; then
    echo "OK: src/us/asm/boot.s assembles byte-exact with baserom 0x1000..0x10FC"
    exit 0
fi

echo "MISMATCH: showing first 16 bytes of diff context"
echo "--- assembled (src/us/asm/boot.s) ---"
xxd "$WORK/boot.bin" | head -20
echo "--- expected (baserom 0x1000) ---"
xxd "$WORK/ref.bin" | head -20
echo "--- byte diff (offset hex) ---"
cmp -l "$WORK/boot.bin" "$WORK/ref.bin" | head -20 || true
exit 1
