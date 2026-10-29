/* 0x00424A5C-0x00424A60: 1 words splat took for the start of a
 * function. Never executed: ldc1 $f20,32($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00424A5C
.type func_00424A5C, @object
func_00424A5C:
    .word 0xD7B40020
.size func_00424A5C, . - func_00424A5C
