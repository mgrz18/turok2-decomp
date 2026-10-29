/* 0x00241004-0x00241010: 3 words splat took for the start of a
 * function. Never executed: lui $1,0x800b; sw $4,-18300($1); beqz $2,2416b4.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00241004
.type func_00241004, @object
func_00241004:
    .word 0x3C01800B
    .word 0xAC24B884
    .word 0x104001A9
.size func_00241004, . - func_00241004
