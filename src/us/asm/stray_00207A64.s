/* 0x00207A64-0x00207A70: 3 words splat took for the start of a
 * function. Never executed: "\0\0\x11m_GameTim".
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00207A64
.type func_00207A64, @object
func_00207A64:
    .word 0x0000116D
    .word 0x5F47616D
    .word 0x6554696D
.size func_00207A64, . - func_00207A64
