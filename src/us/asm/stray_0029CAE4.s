/* 0x0029CAE4-0x0029CAF0: 3 words splat took for the start of a
 * function. Never executed: lwc1, add.s, swc1.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029CAE4
.type func_0029CAE4, @object
func_0029CAE4:
    .word 0xC4206D28
    .word 0x46000800
    .word 0xE600010C
.size func_0029CAE4, . - func_0029CAE4
