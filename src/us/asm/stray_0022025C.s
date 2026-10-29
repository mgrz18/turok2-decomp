/* 0x0022025C-0x00220260: 1 words splat took for the start of a
 * function. Never executed: .word 0x31f019d.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0022025C
.type func_0022025C, @object
func_0022025C:
    .word 0x031F019D
.size func_0022025C, . - func_0022025C
