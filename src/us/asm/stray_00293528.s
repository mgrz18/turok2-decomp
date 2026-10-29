/* 0x00293528-0x00293530: 2 words splat took for the start of a
 * function. Never executed: sw $31,16($29); lw $2,20($4).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293528
.type func_00293528, @object
func_00293528:
    .word 0xAFBF0010
    .word 0x8C820014
.size func_00293528, . - func_00293528
