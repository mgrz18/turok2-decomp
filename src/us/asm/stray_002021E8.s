/* 0x002021E8-0x002021F0: 2 words splat took for the start of a
 * function. Never executed: nop, "\0\x12m_".
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002021E8
.type func_002021E8, @object
func_002021E8:
    .word 0x00000000
    .word 0x00126D5F
.size func_002021E8, . - func_002021E8
