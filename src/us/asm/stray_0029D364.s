/* 0x0029D364-0x0029D370: 3 words splat took for the start of a
 * function. Never executed: lw, lw, ldc1 (an epilogue's loads); the function starts at 0x29D370.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029D364
.type func_0029D364, @object
func_0029D364:
    .word 0x8FB100B4
    .word 0x8FB000B0
    .word 0xD7B400C0
.size func_0029D364, . - func_0029D364
