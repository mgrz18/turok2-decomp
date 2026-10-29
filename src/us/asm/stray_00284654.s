/* 0x00284654-0x00284660: 3 words splat took for the start of a
 * function. Never executed: li, lw, bnez into func_00284660+0xC.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00284654
.type func_00284654, @object
func_00284654:
    .word 0x2413FFFF
    .word 0x8E420000
    .word 0x14400003
.size func_00284654, . - func_00284654
