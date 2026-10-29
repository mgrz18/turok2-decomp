/* 0x0029BA74-0x0029BA80: 3 words splat took for the start of a
 * function. Never executed: lui $1,0x800e; swc1 $f0,-10356($1); jal 2974c0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029BA74
.type func_0029BA74, @object
func_0029BA74:
    .word 0x3C01800E
    .word 0xE420D78C
    .word 0x0C0A5D30
.size func_0029BA74, . - func_0029BA74
