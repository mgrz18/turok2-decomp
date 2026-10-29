/* 0x0029846C-0x00298470: 1 words splat took for the start of a
 * function. Never executed: addiu $5,$4,320.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029846C
.type func_0029846C, @object
func_0029846C:
    .word 0x24850140
.size func_0029846C, . - func_0029846C
