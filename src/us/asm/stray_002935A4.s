/* 0x002935A4-0x002935B0: 3 words splat took for the start of a
 * function. Never executed: addiu $29,$29,24; addiu $29,$29,-24; sw $31,16($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002935A4
.type func_002935A4, @object
func_002935A4:
    .word 0x27BD0018
    .word 0x27BDFFE8
    .word 0xAFBF0010
.size func_002935A4, . - func_002935A4
