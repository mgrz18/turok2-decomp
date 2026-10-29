/* 0x00294534-0x00294540: 3 words splat took for the start of a
 * function. Never executed: or $2,$2,$3; j 25b578; sw $2,0($17).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00294534
.type func_00294534, @object
func_00294534:
    .word 0x00431025
    .word 0x08096D5E
    .word 0xAE220000
.size func_00294534, . - func_00294534
