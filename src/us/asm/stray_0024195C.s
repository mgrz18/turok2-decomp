/* 0x0024195C-0x00241960: 1 words splat took for the start of a
 * function. Never executed: andi $2,$2,0x20.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0024195C
.type func_0024195C, @object
func_0024195C:
    .word 0x30420020
.size func_0024195C, . - func_0024195C
