/* 0x00461C0C-0x00461C10: 1 words splat took for the start of a
 * function. Never executed: addu $4,$2,$3.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00461C0C
.type func_00461C0C, @object
func_00461C0C:
    .word 0x00432021
.size func_00461C0C, . - func_00461C0C
