/* 0x002407FC-0x00240800: 1 words splat took for the start of a
 * function. Never executed: sw $2,0($4).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002407FC
.type func_002407FC, @object
func_002407FC:
    .word 0xAC820000
.size func_002407FC, . - func_002407FC
