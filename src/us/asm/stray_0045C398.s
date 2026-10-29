/* 0x0045C398-0x0045C3A0: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x8013; lw $2,2460($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0045C398
.type func_0045C398, @object
func_0045C398:
    .word 0x3C028013
    .word 0x8C42099C
.size func_0045C398, . - func_0045C398
