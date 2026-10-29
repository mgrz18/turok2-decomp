/* 0x002933A4-0x002933B0: 3 words splat took for the start of a
 * function. Never executed: sll $2,$2,0x10; sra $5,$2,0x10; jal 26d5e4.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002933A4
.type func_002933A4, @object
func_002933A4:
    .word 0x00021400
    .word 0x00022C03
    .word 0x0C09B579
.size func_002933A4, . - func_002933A4
