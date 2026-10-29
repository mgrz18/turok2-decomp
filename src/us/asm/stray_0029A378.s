/* 0x0029A378-0x0029A380: 2 words splat took for the start of a
 * function. Never executed: j 26138c; nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A378
.type func_0029A378, @object
func_0029A378:
    .word 0x080984E3
    .word 0x00000000
.size func_0029A378, . - func_0029A378
