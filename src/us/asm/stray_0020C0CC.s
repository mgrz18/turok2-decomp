/* 0x0020C0CC-0x0020C0D0: 1 words splat took for the start of a
 * function. Never executed: "ISTO".
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0020C0CC
.type func_0020C0CC, @object
func_0020C0CC:
    .word 0x4953544F
.size func_0020C0CC, . - func_0020C0CC
