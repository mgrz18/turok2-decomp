/* 0x0029129C-0x002912A0: 1 words splat took for the start of a
 * function. Never executed: lui $1,0x800a.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029129C
.type func_0029129C, @object
func_0029129C:
    .word 0x3C01800A
.size func_0029129C, . - func_0029129C
