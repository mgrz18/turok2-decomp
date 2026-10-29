/* 0x00427C1C-0x00427C20: 1 words splat took for the start of a
 * function. Never executed: lw $3,-8460($1).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00427C1C
.type func_00427C1C, @object
func_00427C1C:
    .word 0x8C23DEF4
.size func_00427C1C, . - func_00427C1C
