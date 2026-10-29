/* 0x0029A55C-0x0029A560: 1 words splat took for the start of a
 * function. Never executed: mul.s $f1,$f1,$f2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A55C
.type func_0029A55C, @object
func_0029A55C:
    .word 0x46020842
.size func_0029A55C, . - func_0029A55C
