/* 0x0029F178-0x0029F180: 2 words splat took for the start of a
 * function. Never executed: move $6,$3; lhu $3,184($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029F178
.type func_0029F178, @object
func_0029F178:
    .word 0x00603021
    .word 0x960300B8
.size func_0029F178, . - func_0029F178
