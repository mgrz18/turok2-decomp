/* 0x0027BD0C-0x0027BD10: 1 words splat took for the start of a
 * function. Never executed: move $4,$20.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0027BD0C
.type func_0027BD0C, @object
func_0027BD0C:
    .word 0x02802021
.size func_0027BD0C, . - func_0027BD0C
