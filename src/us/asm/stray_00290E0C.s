/* 0x00290E0C-0x00290E10: 1 words splat took for the start of a
 * function. Never executed: li $6,1.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00290E0C
.type func_00290E0C, @object
func_00290E0C:
    .word 0x24060001
.size func_00290E0C, . - func_00290E0C
