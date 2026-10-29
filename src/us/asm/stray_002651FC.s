/* 0x002651FC-0x00265200: 1 words splat took for the start of a
 * function. Never executed: a lone `jal func_0022F1C8`; the data's pointer is 0x00265200.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002651FC
.type func_002651FC, @object
func_002651FC:
    .word 0x0C08BC72
.size func_002651FC, . - func_002651FC
