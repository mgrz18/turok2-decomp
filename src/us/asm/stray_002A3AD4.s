/* 0x002A3AD4-0x002A3AE0: 3 words splat took for the start of a
 * function. Never executed: ahead of __divdi3.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A3AD4
.type func_002A3AD4, @object
func_002A3AD4:
    .word 0x02202021
    .word 0x27A50020
    .word 0x3C01800A
.size func_002A3AD4, . - func_002A3AD4
