/* 0x0029BF74-0x0029BF80: 3 words splat took for the start of a
 * function. Never executed: addiu $2,$30,268; swc1 $f0,32($29); lw $11,24($23).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029BF74
.type func_0029BF74, @object
func_0029BF74:
    .word 0x27C2010C
    .word 0xE7A00020
    .word 0x8EEB0018
.size func_0029BF74, . - func_0029BF74
