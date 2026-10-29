/* 0x002847D8-0x002847E0: 2 words splat took for the start of a
 * function. Never executed: lwc1, sub.s.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002847D8
.type func_002847D8, @object
func_002847D8:
    .word 0xC4206C44
    .word 0x46000801
.size func_002847D8, . - func_002847D8
