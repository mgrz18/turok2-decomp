/* 0x004558B4-0x004558C0: 3 words splat took for the start of a
 * function. Never executed: mfc1 $6,$f0; jal 210318; move $4,$18.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_004558B4
.type func_004558B4, @object
func_004558B4:
    .word 0x44060000
    .word 0x0C0840C6
    .word 0x02402021
.size func_004558B4, . - func_004558B4
