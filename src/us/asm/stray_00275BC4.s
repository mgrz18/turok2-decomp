/* 0x00275BC4-0x00275BE0: 7 words splat took for the start of a
 * function. Never executed: a stale copy of a tail: lw, lw, sw, li, sw, jr ra, sw.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00275BC4
.type func_00275BC4, @object
func_00275BC4:
    .word 0x8C470040
    .word 0x8C480044
    .word 0xAE030018
    .word 0x2402FFFF
    .word 0xAC82000C
    .word 0x03E00008
    .word 0xAC820008
.size func_00275BC4, . - func_00275BC4
