/* 0x0029D444-0x0029D450: 3 words splat took for the start of a
 * function. Never executed: an epilogue (jr ra, addiu sp) and a lw; osCreatePiManager starts at 0x29D450.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029D444
.type func_0029D444, @object
func_0029D444:
    .word 0x03E00008
    .word 0x27BD0020
    .word 0x8C820014
.size func_0029D444, . - func_0029D444
