/* 0x00465EA4-0x00465EB0: 3 words splat took for the start of a
 * function. Never executed: sw $31,20($29); lw $2,28($16); andi $2,$2,0x2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00465EA4
.type func_00465EA4, @object
func_00465EA4:
    .word 0xAFBF0014
    .word 0x8E02001C
    .word 0x30420002
.size func_00465EA4, . - func_00465EA4
