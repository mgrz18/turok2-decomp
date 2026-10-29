/* 0x00257A3C-0x00257A40: 1 words splat took for the start of a
 * function. Never executed: an extra addiu $sp, $sp, -0x90 ahead of the real prologue.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00257A3C
.type func_00257A3C, @object
func_00257A3C:
    .word 0x27BDFF70
.size func_00257A3C, . - func_00257A3C
