/* 0x00430068-0x00430070: 2 words splat took for the start of a
 * function. Never executed: lh $19,8($16); li $2,75.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00430068
.type func_00430068, @object
func_00430068:
    .word 0x86130008
    .word 0x2402004B
.size func_00430068, . - func_00430068
