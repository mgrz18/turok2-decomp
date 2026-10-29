/* 0x004660D8-0x004660E0: 2 words splat took for the start of a
 * function. Never executed: li $2,-1; jal 291bb0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_004660D8
.type func_004660D8, @object
func_004660D8:
    .word 0x2402FFFF
    .word 0x0C0A46EC
.size func_004660D8, . - func_004660D8
