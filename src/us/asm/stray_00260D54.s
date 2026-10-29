/* 0x00260D54-0x00260D60: 3 words splat took for the start of a
 * function. Never executed: lw $9,212($29); sw $3,100($29); sw $8,104($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00260D54
.type func_00260D54, @object
func_00260D54:
    .word 0x8FA900D4
    .word 0xAFA30064
    .word 0xAFA80068
.size func_00260D54, . - func_00260D54
