/* 0x002A4BD4-0x002A4BE0: 3 words splat took for the start of a
 * function. Never executed: ahead of __umoddi3.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A4BD4
.type func_002A4BD4, @object
func_002A4BD4:
    .word 0x00042080
    .word 0x00E42021
    .word 0x00051FC3
.size func_002A4BD4, . - func_002A4BD4
