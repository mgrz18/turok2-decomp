/* 0x00297FAC-0x00297FB0: 1 words splat took for the start of a
 * function. Never executed: a bne; guPerspective starts at 0x297FB0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00297FAC
.type func_00297FAC, @object
func_00297FAC:
    .word 0x1483000D
.size func_00297FAC, . - func_00297FAC
