/* 0x0040C84C-0x0040C850: 1 words splat took for the start of a
 * function. Never executed: addu $2,$3,$17.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0040C84C
.type func_0040C84C, @object
func_0040C84C:
    .word 0x00711021
.size func_0040C84C, . - func_0040C84C
