/* 0x0029A158-0x0029A160: 2 words splat took for the start of a
 * function. Never executed: addiu $29,$29,24; addiu $29,$29,-248.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A158
.type func_0029A158, @object
func_0029A158:
    .word 0x27BD0018
    .word 0x27BDFF08
.size func_0029A158, . - func_0029A158
