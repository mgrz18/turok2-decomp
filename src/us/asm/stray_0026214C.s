/* 0x0026214C-0x00262150: 1 words splat took for the start of a
 * function. Never executed: mult $2,$7.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026214C
.type func_0026214C, @object
func_0026214C:
    .word 0x00470018
.size func_0026214C, . - func_0026214C
