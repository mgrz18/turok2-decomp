/* 0x00206918-0x00206920: 2 words splat took for the start of a
 * function. Never executed: "sh" and a data word.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00206918
.type func_00206918, @object
func_00206918:
    .word 0x73680000
    .word 0x00940800
.size func_00206918, . - func_00206918
