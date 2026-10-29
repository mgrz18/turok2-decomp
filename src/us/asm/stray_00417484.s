/* 0x00417484-0x00417490: 3 words splat took for the start of a
 * function. Never executed: move $4,$2; jal 2017d4; move $5,$18.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00417484
.type func_00417484, @object
func_00417484:
    .word 0x00402021
    .word 0x0C0805F5
    .word 0x02402821
.size func_00417484, . - func_00417484
