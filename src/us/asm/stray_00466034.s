/* 0x00466034-0x00466040: 3 words splat took for the start of a
 * function. Never executed: jr ra; addiu sp,32; li $2,2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00466034
.type func_00466034, @object
func_00466034:
    .word 0x03E00008
    .word 0x27BD0020
    .word 0x24020002
.size func_00466034, . - func_00466034
