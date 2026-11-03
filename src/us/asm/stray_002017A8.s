/* 0x002017A8-0x002017B0: 2 words splat took for the start of a
 * function. Never executed: "meSkyLay".
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002017A8
.type func_002017A8, @object
func_002017A8:
    .word 0x6D65536B
    .word 0x794C6179
.size func_002017A8, . - func_002017A8
