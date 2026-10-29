/* 0x0029AC34-0x0029AC40: 3 words splat took for the start of a
 * function. Never executed: addiu $29,$29,64; addiu $29,$29,-40; sw $17,20($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029AC34
.type func_0029AC34, @object
func_0029AC34:
    .word 0x27BD0040
    .word 0x27BDFFD8
    .word 0xAFB10014
.size func_0029AC34, . - func_0029AC34
