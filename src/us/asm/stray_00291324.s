/* 0x00291324-0x00291330: 3 words splat took for the start of a
 * function. Never executed: swc1, lh, jal func_0024E700; alCSPNew starts at 0x291330.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00291324
.type func_00291324, @object
func_00291324:
    .word 0xE421E350
    .word 0x84850996
    .word 0x0C0939C0
.size func_00291324, . - func_00291324
