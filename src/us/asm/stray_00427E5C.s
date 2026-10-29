/* 0x00427E5C-0x00427E60: 1 words splat took for the start of a
 * function. Never executed: beqzl $2,427ea4.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00427E5C
.type func_00427E5C, @object
func_00427E5C:
    .word 0x50400011
.size func_00427E5C, . - func_00427E5C
