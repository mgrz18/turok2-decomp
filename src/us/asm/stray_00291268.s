/* 0x00291268-0x00291270: 2 words splat took for the start of a
 * function. Never executed: c.lt.s, nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00291268
.type func_00291268, @object
func_00291268:
    .word 0x4602083C
    .word 0x00000000
.size func_00291268, . - func_00291268
