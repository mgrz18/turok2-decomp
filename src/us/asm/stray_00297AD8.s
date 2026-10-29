/* 0x00297AD8-0x00297AE0: 2 words splat took for the start of a
 * function. Never executed: bne into func_00297AE0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00297AD8
.type func_00297AD8, @object
func_00297AD8:
    .word 0x244218C0
    .word 0x14620003
.size func_00297AD8, . - func_00297AD8
