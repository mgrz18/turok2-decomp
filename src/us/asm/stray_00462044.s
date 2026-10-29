/* 0x00462044-0x00462050: 3 words splat took for the start of a
 * function. Never executed: lw $20,4($19); beqz $20,4620f0; li $17,-1.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00462044
.type func_00462044, @object
func_00462044:
    .word 0x8E740004
    .word 0x12800029
    .word 0x2411FFFF
.size func_00462044, . - func_00462044
