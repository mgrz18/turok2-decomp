/* 0x00441FF8-0x00442000: 2 words splat took for the start of a
 * function. Never executed: mov.s $f20,$f0; lui $1,0x800a.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00441FF8
.type func_00441FF8, @object
func_00441FF8:
    .word 0x46000506
    .word 0x3C01800A
.size func_00441FF8, . - func_00441FF8
