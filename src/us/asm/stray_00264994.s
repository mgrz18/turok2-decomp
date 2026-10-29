/* 0x00264994-0x002649A0: 3 words splat took for the start of a
 * function. Never executed: the only reference, func_00264C94, stores func_00264994 + 0xC.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00264994
.type func_00264994, @object
func_00264994:
    .word 0x8C421CE0
    .word 0x10400010
    .word 0x3C030100
.size func_00264994, . - func_00264994
