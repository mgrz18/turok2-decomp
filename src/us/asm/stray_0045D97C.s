/* 0x0045D97C-0x0045D980: 1 words splat took for the start of a
 * function. Never executed: lw $4,24($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0045D97C
.type func_0045D97C, @object
func_0045D97C:
    .word 0x8E040018
.size func_0045D97C, . - func_0045D97C
