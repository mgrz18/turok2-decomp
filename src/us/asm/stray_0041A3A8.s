/* 0x0041A3A8-0x0041A3B0: 2 words splat took for the start of a
 * function. Never executed: a nop and a bc1f word never executed; the function starts at 0x41A3B0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0041A3A8
.type func_0041A3A8, @object
func_0041A3A8:
    .word 0x00000000
    .word 0x45000038
.size func_0041A3A8, . - func_0041A3A8
