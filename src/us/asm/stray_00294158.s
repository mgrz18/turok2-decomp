/* 0x00294158-0x00294160: 2 words splat took for the start of a
 * function. Never executed: jal 2113a4; nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00294158
.type func_00294158, @object
func_00294158:
    .word 0x0C0844E9
    .word 0x00000000
.size func_00294158, . - func_00294158
