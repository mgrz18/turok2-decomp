/* 0x00468F88-0x00468F90: 2 words splat took for the start of a
 * function. Never executed: mtc1 $7,$f0; cvt.s.w $f0,$f0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00468F88
.type func_00468F88, @object
func_00468F88:
    .word 0x44870000
    .word 0x46800020
.size func_00468F88, . - func_00468F88
