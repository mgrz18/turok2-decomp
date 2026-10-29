/* 0x00460B3C-0x00460B40: 1 words splat took for the start of a
 * function. Never executed: subu $4,$4,$2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00460B3C
.type func_00460B3C, @object
func_00460B3C:
    .word 0x00822023
.size func_00460B3C, . - func_00460B3C
