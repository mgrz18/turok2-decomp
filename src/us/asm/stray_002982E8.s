/* 0x002982E8-0x002982F0: 2 words splat took for the start of a
 * function. Never executed: ori $3,$3,0xffff; and $2,$2,$3.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002982E8
.type func_002982E8, @object
func_002982E8:
    .word 0x3463FFFF
    .word 0x00431024
.size func_002982E8, . - func_002982E8
