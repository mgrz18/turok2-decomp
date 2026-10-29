/* 0x0027BB7C-0x0027BB80: 1 words splat took for the start of a
 * function. Never executed: sw $0,208($17).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0027BB7C
.type func_0027BB7C, @object
func_0027BB7C:
    .word 0xAE2000D0
.size func_0027BB7C, . - func_0027BB7C
