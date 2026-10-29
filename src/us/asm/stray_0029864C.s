/* 0x0029864C-0x00298650: 1 words splat took for the start of a
 * function. Never executed: mov.s $f12,$f20.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029864C
.type func_0029864C, @object
func_0029864C:
    .word 0x4600A306
.size func_0029864C, . - func_0029864C
