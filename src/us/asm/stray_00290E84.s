/* 0x00290E84-0x00290E90: 3 words splat took for the start of a
 * function. Never executed: move $5,$18; lw $31,28($29); lw $18,24($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00290E84
.type func_00290E84, @object
func_00290E84:
    .word 0x02402821
    .word 0x8FBF001C
    .word 0x8FB20018
.size func_00290E84, . - func_00290E84
