/* 0x00299F84-0x00299F90: 3 words splat took for the start of a
 * function. Never executed: bc1f 299fac; nop; j 260fc0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00299F84
.type func_00299F84, @object
func_00299F84:
    .word 0x45000009
    .word 0x00000000
    .word 0x080983F0
.size func_00299F84, . - func_00299F84
