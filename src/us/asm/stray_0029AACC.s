/* 0x0029AACC-0x0029AAD0: 1 words splat took for the start of a
 * function. Never executed: lwc1 $f14,31500($1).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029AACC
.type func_0029AACC, @object
func_0029AACC:
    .word 0xC42E7B0C
.size func_0029AACC, . - func_0029AACC
