/* 0x002768C8-0x002768D0: 2 words splat took for the start of a
 * function. Never executed: srl $2,$2,0x9; sll $4,$2,0x2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002768C8
.type func_002768C8, @object
func_002768C8:
    .word 0x00021242
    .word 0x00022080
.size func_002768C8, . - func_002768C8
