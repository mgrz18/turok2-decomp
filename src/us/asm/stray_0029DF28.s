/* 0x0029DF28-0x0029DF30: 2 words splat took for the start of a
 * function. Never executed: lb $3,372($16); beq $3,$2,29df40.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DF28
.type func_0029DF28, @object
func_0029DF28:
    .word 0x82030174
    .word 0x10620004
.size func_0029DF28, . - func_0029DF28
