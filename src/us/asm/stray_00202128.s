/* 0x00202128-0x00202130: 2 words splat took for the start of a
 * function. Never executed: "Style", two bytes; func_00286674 calls 0x202130.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00202128
.type func_00202128, @object
func_00202128:
    .word 0x5374796C
    .word 0x65B88F01
.size func_00202128, . - func_00202128
