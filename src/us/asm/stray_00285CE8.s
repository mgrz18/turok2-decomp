/* 0x00285CE8-0x00285CF0: 2 words splat took for the start of a
 * function. Never executed: swc1, sw; the pointer in AB858.rodata.s is 0x00285CF0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00285CE8
.type func_00285CE8, @object
func_00285CE8:
    .word 0xE7B50010
    .word 0xAFB00014
.size func_00285CE8, . - func_00285CE8
