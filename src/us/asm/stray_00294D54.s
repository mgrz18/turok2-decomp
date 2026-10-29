/* 0x00294D54-0x00294D60: 3 words splat took for the start of a
 * function. Never executed: addiu $2,$2,-13624; sw $2,256($16); lui $2,0x26.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00294D54
.type func_00294D54, @object
func_00294D54:
    .word 0x2442CAC8
    .word 0xAE020100
    .word 0x3C020026
.size func_00294D54, . - func_00294D54
