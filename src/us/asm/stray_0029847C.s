/* 0x0029847C-0x00298480: 1 words splat took for the start of a
 * function. Never executed: lb $2,199($17).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029847C
.type func_0029847C, @object
func_0029847C:
    .word 0x822200C7
.size func_0029847C, . - func_0029847C
