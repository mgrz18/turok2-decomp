/* 0x0029DED8-0x0029DEE0: 2 words splat took for the start of a
 * function. Never executed: mtc1 $2,$f0; cvt.s.w $f0,$f0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DED8
.type func_0029DED8, @object
func_0029DED8:
    .word 0x44820000
    .word 0x46800020
.size func_0029DED8, . - func_0029DED8
