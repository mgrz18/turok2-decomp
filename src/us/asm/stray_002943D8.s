/* 0x002943D8-0x002943E0: 2 words splat took for the start of a
 * function. Never executed: bne into alSynSetVol.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002943D8
.type func_002943D8, @object
func_002943D8:
    .word 0x14620018
    .word 0x00000000
.size func_002943D8, . - func_002943D8
