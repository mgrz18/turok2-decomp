/* 0x00294148-0x00294150: 2 words splat took for the start of a
 * function. Never executed: li $2,-1; lb $3,80($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00294148
.type func_00294148, @object
func_00294148:
    .word 0x2402FFFF
    .word 0x82030050
.size func_00294148, . - func_00294148
