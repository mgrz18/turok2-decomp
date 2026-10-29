/* 0x0041A4CC-0x0041A4D0: 1 words splat took for the start of a
 * function. Never executed: a lone jr ra; splat took the function's first word for its delay slot.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0041A4CC
.type func_0041A4CC, @object
func_0041A4CC:
    .word 0x03E00008
.size func_0041A4CC, . - func_0041A4CC
