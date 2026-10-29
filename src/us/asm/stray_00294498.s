/* 0x00294498-0x002944A0: 2 words splat took for the start of a
 * function. Never executed: and $2,$2,$3; sw $2,0($17).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00294498
.type func_00294498, @object
func_00294498:
    .word 0x00431024
    .word 0xAE220000
.size func_00294498, . - func_00294498
