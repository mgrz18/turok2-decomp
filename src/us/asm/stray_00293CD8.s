/* 0x00293CD8-0x00293CE0: 2 words splat took for the start of a
 * function. Never executed: j 25ad24; addiu $2,$2,100.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293CD8
.type func_00293CD8, @object
func_00293CD8:
    .word 0x08096B49
    .word 0x24420064
.size func_00293CD8, . - func_00293CD8
