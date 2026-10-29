/* 0x00298718-0x00298720: 2 words splat took for the start of a
 * function. Never executed: sb $0,307($5); jal 243414.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00298718
.type func_00298718, @object
func_00298718:
    .word 0xA0A00133
    .word 0x0C090D05
.size func_00298718, . - func_00298718
