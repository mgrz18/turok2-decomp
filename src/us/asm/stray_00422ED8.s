/* 0x00422ED8-0x00422EE0: 2 words splat took for the start of a
 * function. Never executed: addiu $29,$29,-24; sw $31,16($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00422ED8
.type func_00422ED8, @object
func_00422ED8:
    .word 0x27BDFFE8
    .word 0xAFBF0010
.size func_00422ED8, . - func_00422ED8
