/* 0x0026D1F8-0x0026D200: 2 words splat took for the start of a
 * function. Never executed: sw $9,4($2); sw $10,8($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026D1F8
.type func_0026D1F8, @object
func_0026D1F8:
    .word 0xAC490004
    .word 0xAC4A0008
.size func_0026D1F8, . - func_0026D1F8
