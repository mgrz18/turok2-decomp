/* 0x002974B4-0x002974C0: 3 words splat took for the start of a
 * function. Never executed: addiu $29,$29,24; addiu $29,$29,-32; sw $17,20($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002974B4
.type func_002974B4, @object
func_002974B4:
    .word 0x27BD0018
    .word 0x27BDFFE0
    .word 0xAFB10014
.size func_002974B4, . - func_002974B4
