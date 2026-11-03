/* 0x002A51E0-0x002A51F8: 6 words splat took for the start of a
 * function. Never executed: "header", three words, "texture": the end of .code, after __floatdisf.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A51E0
.type func_002A51E0, @object
func_002A51E0:
    .word 0x68656164
    .word 0x6572000F
    .word 0x00001021
    .word 0xC4400020
    .word 0x74657874
    .word 0x75726500
.size func_002A51E0, . - func_002A51E0
