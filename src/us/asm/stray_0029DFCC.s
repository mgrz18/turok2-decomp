/* 0x0029DFCC-0x0029DFD0: 1 words splat took for the start of a
 * function. Never executed: lw $5,4($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DFCC
.type func_0029DFCC, @object
func_0029DFCC:
    .word 0x8E050004
.size func_0029DFCC, . - func_0029DFCC
