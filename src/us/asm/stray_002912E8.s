/* 0x002912E8-0x002912F0: 2 words splat took for the start of a
 * function. Never executed: c.lt.s, nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002912E8
.type func_002912E8, @object
func_002912E8:
    .word 0x4601003C
    .word 0x00000000
.size func_002912E8, . - func_002912E8
