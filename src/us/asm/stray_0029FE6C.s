/* 0x0029FE6C-0x0029FE70: 1 words splat took for the start of a
 * function. Never executed: sw $3,0($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029FE6C
.type func_0029FE6C, @object
func_0029FE6C:
    .word 0xAC430000
.size func_0029FE6C, . - func_0029FE6C
