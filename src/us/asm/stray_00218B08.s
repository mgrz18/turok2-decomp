/* 0x00218B08-0x00218B10: 2 words splat took for the start of a
 * function. Never executed: sll $0,$4,0x0; mthi $0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00218B08
.type func_00218B08, @object
func_00218B08:
    .word 0x00040000
    .word 0x00000011
.size func_00218B08, . - func_00218B08
