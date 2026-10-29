/* 0x004304FC-0x00430500: 1 words splat took for the start of a
 * function. Never executed: swc1 $f0,2604($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_004304FC
.type func_004304FC, @object
func_004304FC:
    .word 0xE6000A2C
.size func_004304FC, . - func_004304FC
