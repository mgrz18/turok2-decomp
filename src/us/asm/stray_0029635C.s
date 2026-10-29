/* 0x0029635C-0x00296360: 1 words splat took for the start of a
 * function. Never executed: a bnez; alMainBusPull starts at 0x296360.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029635C
.type func_0029635C, @object
func_0029635C:
    .word 0x14400008
.size func_0029635C, . - func_0029635C
