/* 0x00290F1C-0x00290F20: 1 words splat took for the start of a
 * function. Never executed: li $2,23.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00290F1C
.type func_00290F1C, @object
func_00290F1C:
    .word 0x24020017
.size func_00290F1C, . - func_00290F1C
