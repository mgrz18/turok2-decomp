/* 0x00241854-0x00241860: 3 words splat took for the start of a
 * function. Never executed: lui $2,0x8012; lw $2,-21800($2); andi $2,$2,0x400.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00241854
.type func_00241854, @object
func_00241854:
    .word 0x3C028012
    .word 0x8C42AAD8
    .word 0x30420400
.size func_00241854, . - func_00241854
