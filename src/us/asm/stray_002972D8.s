/* 0x002972D8-0x002972E0: 2 words splat took for the start of a
 * function. Never executed: jal func_0025AAA4, addu; alSavePull starts at 0x2972E0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002972D8
.type func_002972D8, @object
func_002972D8:
    .word 0x0C096AA9
    .word 0x02202821
.size func_002972D8, . - func_002972D8
