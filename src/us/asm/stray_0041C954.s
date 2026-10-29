/* 0x0041C954-0x0041C960: 3 words splat took for the start of a
 * function. Never executed: li $4,1; li $2,8; beql $3,$2,41c970.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0041C954
.type func_0041C954, @object
func_0041C954:
    .word 0x24040001
    .word 0x24020008
    .word 0x50620004
.size func_0041C954, . - func_0041C954
