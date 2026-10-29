/* 0x0029BC64-0x0029BC70: 3 words splat took for the start of a
 * function. Never executed: li, beq, slt.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029BC64
.type func_0029BC64, @object
func_0029BC64:
    .word 0x240205F0
    .word 0x10820034
    .word 0x0082102A
.size func_0029BC64, . - func_0029BC64
