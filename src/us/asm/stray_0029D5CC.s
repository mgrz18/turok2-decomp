/* 0x0029D5CC-0x0029D5D0: 1 words splat took for the start of a
 * function. Never executed: lui $1,0x800b.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029D5CC
.type func_0029D5CC, @object
func_0029D5CC:
    .word 0x3C01800B
.size func_0029D5CC, . - func_0029D5CC
