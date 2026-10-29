/* 0x00461938-0x00461940: 2 words splat took for the start of a
 * function. Never executed: lh $5,11148($17); jal 274008.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00461938
.type func_00461938, @object
func_00461938:
    .word 0x86252B8C
    .word 0x0C09D002
.size func_00461938, . - func_00461938
