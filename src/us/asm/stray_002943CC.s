/* 0x002943CC-0x002943D0: 1 words splat took for the start of a
 * function. Never executed: j 25b4cc.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002943CC
.type func_002943CC, @object
func_002943CC:
    .word 0x08096D33
.size func_002943CC, . - func_002943CC
