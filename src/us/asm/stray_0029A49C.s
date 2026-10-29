/* 0x0029A49C-0x0029A4A0: 1 words splat took for the start of a
 * function. Never executed: sw $31,24($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A49C
.type func_0029A49C, @object
func_0029A49C:
    .word 0xAFBF0018
.size func_0029A49C, . - func_0029A49C
