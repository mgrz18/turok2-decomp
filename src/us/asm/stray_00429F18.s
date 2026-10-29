/* 0x00429F18-0x00429F20: 2 words splat took for the start of a
 * function. Never executed: sw $3,16($29); sw $8,20($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00429F18
.type func_00429F18, @object
func_00429F18:
    .word 0xAFA30010
    .word 0xAFA80014
.size func_00429F18, . - func_00429F18
