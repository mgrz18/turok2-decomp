/* 0x0045C688-0x0045C690: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x8013; lw $2,2448($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0045C688
.type func_0045C688, @object
func_0045C688:
    .word 0x3C028013
    .word 0x8C420990
.size func_0045C688, . - func_0045C688
