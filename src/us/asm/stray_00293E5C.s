/* 0x00293E5C-0x00293E60: 1 words splat took for the start of a
 * function. Never executed: lw $2,128($18).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293E5C
.type func_00293E5C, @object
func_00293E5C:
    .word 0x8E420080
.size func_00293E5C, . - func_00293E5C
