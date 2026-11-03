/* 0x00293E58-0x00293E60: 2 words splat took for the start of a
 * function. Never executed: nop; lw $2,128($18).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293E58
.type func_00293E58, @object
func_00293E58:
    .word 0x00000000
    .word 0x8E420080
.size func_00293E58, . - func_00293E58
