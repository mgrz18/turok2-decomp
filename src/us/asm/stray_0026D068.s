/* 0x0026D068-0x0026D070: 2 words splat took for the start of a
 * function. Never executed: nop, lwc1 $f1,24($4).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026D068
.type func_0026D068, @object
func_0026D068:
    .word 0x00000000
    .word 0xC4810018
.size func_0026D068, . - func_0026D068
