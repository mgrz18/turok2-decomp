/* 0x0041D04C-0x0041D050: 1 words splat took for the start of a
 * function. Never executed: lb $3,406($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0041D04C
.type func_0041D04C, @object
func_0041D04C:
    .word 0x82030196
.size func_0041D04C, . - func_0041D04C
