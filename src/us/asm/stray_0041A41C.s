/* 0x0041A41C-0x0041A420: 1 words splat took for the start of a
 * function. Never executed: move $5,$17.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0041A41C
.type func_0041A41C, @object
func_0041A41C:
    .word 0x02202821
.size func_0041A41C, . - func_0041A41C
