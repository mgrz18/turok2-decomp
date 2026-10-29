/* 0x0041D884-0x0041D890: 3 words splat took for the start of a
 * function. Never executed: bne $3,$2,41d9c8; addiu $16,$29,48; addiu $4,$29,72.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0041D884
.type func_0041D884, @object
func_0041D884:
    .word 0x14620050
    .word 0x27B00030
    .word 0x27A40048
.size func_0041D884, . - func_0041D884
