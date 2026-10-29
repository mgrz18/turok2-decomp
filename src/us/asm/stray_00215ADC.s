/* 0x00215ADC-0x00215AE0: 1 words splat took for the start of a
 * function. Never executed: nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00215ADC
.type func_00215ADC, @object
func_00215ADC:
    .word 0x00000000
.size func_00215ADC, . - func_00215ADC
