/* 0x0029A4CC-0x0029A4D0: 1 words splat took for the start of a
 * function. Never executed: c.lt.s $f0,$f14.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A4CC
.type func_0029A4CC, @object
func_0029A4CC:
    .word 0x460E003C
.size func_0029A4CC, . - func_0029A4CC
