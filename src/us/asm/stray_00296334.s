/* 0x00296334-0x00296340: 3 words splat took for the start of a
 * function. Never executed: lhu, andi, bnez into alMainBusPull+0x20.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00296334
.type func_00296334, @object
func_00296334:
    .word 0x9442007C
    .word 0x3042001F
    .word 0x14400010
.size func_00296334, . - func_00296334
