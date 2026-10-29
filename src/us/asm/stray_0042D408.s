/* 0x0042D408-0x0042D410: 2 words splat took for the start of a
 * function. Never executed: addiu $4,$29,32; addiu $5,$29,84.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042D408
.type func_0042D408, @object
func_0042D408:
    .word 0x27A40020
    .word 0x27A50054
.size func_0042D408, . - func_0042D408
