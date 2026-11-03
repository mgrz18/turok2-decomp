/* 0x00200C38-0x00200C40: 2 words splat took for the start of a
 * function. Never executed: "oosh", nop: ahead of the hand-written RNC code.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00200C38
.type func_00200C38, @object
func_00200C38:
    .word 0x6F6F7368
    .word 0x00000000
.size func_00200C38, . - func_00200C38
