/* 0x0041A3AC-0x0041A3B0: 1 words splat took for the start of a
 * function. Never executed: a bc1f word never executed; the function starts at 0x41A3B0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0041A3AC
.type func_0041A3AC, @object
func_0041A3AC:
    .word 0x45000038
.size func_0041A3AC, . - func_0041A3AC
