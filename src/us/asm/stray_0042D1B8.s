/* 0x0042D1B8-0x0042D1C0: 2 words splat took for the start of a
 * function. Never executed: move $5,$6; move $6,$17.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042D1B8
.type func_0042D1B8, @object
func_0042D1B8:
    .word 0x00C02821
    .word 0x02203021
.size func_0042D1B8, . - func_0042D1B8
