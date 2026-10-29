/* 0x0029E334-0x0029E340: 3 words splat took for the start of a
 * function. Never executed: lui $4,0x800f; addiu $4,$4,28792; move $5,$16.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E334
.type func_0029E334, @object
func_0029E334:
    .word 0x3C04800F
    .word 0x24847078
    .word 0x02002821
.size func_0029E334, . - func_0029E334
