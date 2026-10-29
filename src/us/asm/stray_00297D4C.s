/* 0x00297D4C-0x00297D50: 1 words splat took for the start of a
 * function. Never executed: addiu $29,$29,-24.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00297D4C
.type func_00297D4C, @object
func_00297D4C:
    .word 0x27BDFFE8
.size func_00297D4C, . - func_00297D4C
