/* 0x0029B4A4-0x0029B4B0: 3 words splat took for the start of a
 * function. Never executed: neg.s, c.lt.s, nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B4A4
.type func_0029B4A4, @object
func_0029B4A4:
    .word 0x46001807
    .word 0x4601003C
    .word 0x00000000
.size func_0029B4A4, . - func_0029B4A4
