/* 0x00290D8C-0x00290D90: 1 words splat took for the start of a
 * function. Never executed: lh $4,-8344($1).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00290D8C
.type func_00290D8C, @object
func_00290D8C:
    .word 0x8424DF68
.size func_00290D8C, . - func_00290D8C
