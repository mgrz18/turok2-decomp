/* 0x0029E2D8-0x0029E2E0: 2 words splat took for the start of a
 * function. Never executed: cvt.s.w $f0,$f0; mul.s $f0,$f0,$f1.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E2D8
.type func_0029E2D8, @object
func_0029E2D8:
    .word 0x46800020
    .word 0x46010002
.size func_0029E2D8, . - func_0029E2D8
