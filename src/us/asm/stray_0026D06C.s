/* 0x0026D06C-0x0026D070: 1 words splat took for the start of a
 * function. Never executed: lwc1 $f1,24($4).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026D06C
.type func_0026D06C, @object
func_0026D06C:
    .word 0xC4810018
.size func_0026D06C, . - func_0026D06C
