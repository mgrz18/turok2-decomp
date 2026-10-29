/* 0x00267088-0x00267090: 2 words splat took for the start of a
 * function. Never executed: ldc1 $f20,232($29); jr $31.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00267088
.type func_00267088, @object
func_00267088:
    .word 0xD7B400E8
    .word 0x03E00008
.size func_00267088, . - func_00267088
