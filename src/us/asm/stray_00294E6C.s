/* 0x00294E6C-0x00294E70: 1 words splat took for the start of a
 * function. Never executed: sw $2,212($17).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00294E6C
.type func_00294E6C, @object
func_00294E6C:
    .word 0xAE2200D4
.size func_00294E6C, . - func_00294E6C
