/* 0x00462374-0x00462380: 3 words splat took for the start of a
 * function. Never executed: j 271784; nop; jal 29ecd0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00462374
.type func_00462374, @object
func_00462374:
    .word 0x0809C5E1
    .word 0x00000000
    .word 0x0C0A7B34
.size func_00462374, . - func_00462374
