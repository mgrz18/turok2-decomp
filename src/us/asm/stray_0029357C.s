/* 0x0029357C-0x00293580: 1 words splat took for the start of a
 * function. Never executed: addiu $29,$29,-24.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029357C
.type func_0029357C, @object
func_0029357C:
    .word 0x27BDFFE8
.size func_0029357C, . - func_0029357C
