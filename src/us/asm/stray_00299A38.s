/* 0x00299A38-0x00299A40: 2 words splat took for the start of a
 * function. Never executed: sw $18,232($29); move $18,$4.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00299A38
.type func_00299A38, @object
func_00299A38:
    .word 0xAFB200E8
    .word 0x00809021
.size func_00299A38, . - func_00299A38
