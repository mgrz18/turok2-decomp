/* 0x00265F84-0x00265F90: 3 words splat took for the start of a
 * function. Never executed: lui $16,0x800f; addiu $16,$16,6160; move $4,$16.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00265F84
.type func_00265F84, @object
func_00265F84:
    .word 0x3C10800F
    .word 0x26101810
    .word 0x02002021
.size func_00265F84, . - func_00265F84
