/* 0x002941FC-0x00294200: 1 words splat took for the start of a
 * function. Never executed: lb $3,84($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002941FC
.type func_002941FC, @object
func_002941FC:
    .word 0x82030054
.size func_002941FC, . - func_002941FC
