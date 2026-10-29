/* 0x00259E58-0x00259E60: 2 words splat took for the start of a
 * function. Never executed: li $6,1; li $2,144.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00259E58
.type func_00259E58, @object
func_00259E58:
    .word 0x24060001
    .word 0x24020090
.size func_00259E58, . - func_00259E58
