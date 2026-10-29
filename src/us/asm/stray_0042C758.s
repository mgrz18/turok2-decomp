/* 0x0042C758-0x0042C760: 2 words splat took for the start of a
 * function. Never executed: addiu $29,$29,-24; sw $16,16($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042C758
.type func_0042C758, @object
func_0042C758:
    .word 0x27BDFFE8
    .word 0xAFB00010
.size func_0042C758, . - func_0042C758
