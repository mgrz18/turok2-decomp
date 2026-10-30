/* 0x0029D5C8-0x0029D5D0: 2 words splat took for the start of a
 * function. Never executed: nop, lui $1,0x800b.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029D5C8
.type func_0029D5C8, @object
func_0029D5C8:
    .word 0x00000000
    .word 0x3C01800B
.size func_0029D5C8, . - func_0029D5C8
