/* 0x00299674-0x00299680: 3 words splat took for the start of a
 * function. Never executed: jal 25eb24; move $5,$16; move $4,$17.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00299674
.type func_00299674, @object
func_00299674:
    .word 0x0C097AC9
    .word 0x02002821
    .word 0x02202021
.size func_00299674, . - func_00299674
