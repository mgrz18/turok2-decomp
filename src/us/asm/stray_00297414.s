/* 0x00297414-0x00297420: 3 words splat took for the start of a
 * function. Never executed: lw $18,32($29); lw $17,28($29); lw $16,24($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00297414
.type func_00297414, @object
func_00297414:
    .word 0x8FB20020
    .word 0x8FB1001C
    .word 0x8FB00018
.size func_00297414, . - func_00297414
