/* 0x00277088-0x00277090: 2 words splat took for the start of a
 * function. Never executed: bne, nop; callers jal 0x277090.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00277088
.type func_00277088, @object
func_00277088:
    .word 0x1482000D
    .word 0x00000000
.size func_00277088, . - func_00277088
