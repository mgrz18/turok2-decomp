/* 0x00299EA8-0x00299EB0: 2 words splat took for the start of a
 * function. Never executed: bc1f into osPiRawStartDma.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00299EA8
.type func_00299EA8, @object
func_00299EA8:
    .word 0x45000009
    .word 0x00000000
.size func_00299EA8, . - func_00299EA8
