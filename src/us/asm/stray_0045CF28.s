/* 0x0045CF28-0x0045CF30: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x8013; lh $2,2512($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0045CF28
.type func_0045CF28, @object
func_0045CF28:
    .word 0x3C028013
    .word 0x844209D0
.size func_0045CF28, . - func_0045CF28
