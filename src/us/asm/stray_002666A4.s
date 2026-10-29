/* 0x002666A4-0x002666B0: 3 words splat took for the start of a
 * function. Never executed: sw $8,416($29); sw $9,420($29); sw $10,424($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002666A4
.type func_002666A4, @object
func_002666A4:
    .word 0xAFA801A0
    .word 0xAFA901A4
    .word 0xAFAA01A8
.size func_002666A4, . - func_002666A4
