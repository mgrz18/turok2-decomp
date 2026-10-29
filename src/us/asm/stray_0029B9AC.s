/* 0x0029B9AC-0x0029B9B0: 1 words splat took for the start of a
 * function. Never executed: move $16,$4.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B9AC
.type func_0029B9AC, @object
func_0029B9AC:
    .word 0x00808021
.size func_0029B9AC, . - func_0029B9AC
