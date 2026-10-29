/* 0x0029DFEC-0x0029DFF0: 1 words splat took for the start of a
 * function. Never executed: lui $1,0x800a.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DFEC
.type func_0029DFEC, @object
func_0029DFEC:
    .word 0x3C01800A
.size func_0029DFEC, . - func_0029DFEC
