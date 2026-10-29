/* 0x002A4648-0x002A4650: 2 words splat took for the start of a
 * function. Never executed: ahead of __udivdi3.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A4648
.type func_002A4648, @object
func_002A4648:
    .word 0x46011080
    .word 0x4617103C
.size func_002A4648, . - func_002A4648
