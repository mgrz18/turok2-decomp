/* 0x0045337C-0x00453380: 1 words splat took for the start of a
 * function. Never executed: a c.lt.s never executed; the function starts 16-aligned.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0045337C
.type func_0045337C, @object
func_0045337C:
    .word 0x4601183C
.size func_0045337C, . - func_0045337C
