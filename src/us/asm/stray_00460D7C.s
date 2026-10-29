/* 0x00460D7C-0x00460D80: 1 words splat took for the start of a
 * function. Never executed: lw $16,0($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00460D7C
.type func_00460D7C, @object
func_00460D7C:
    .word 0x8C500000
.size func_00460D7C, . - func_00460D7C
