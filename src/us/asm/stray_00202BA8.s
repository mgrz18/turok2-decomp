/* 0x00202BA8-0x00202BB0: 2 words splat took for the start of a
 * function. Never executed: two data words.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00202BA8
.type func_00202BA8, @object
func_00202BA8:
    .word 0x96660000
    .word 0x00280000
.size func_00202BA8, . - func_00202BA8
