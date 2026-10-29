/* 0x0029B16C-0x0029B170: 1 words splat took for the start of a
 * function. Never executed: sw $18,136($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B16C
.type func_0029B16C, @object
func_0029B16C:
    .word 0xAFB20088
.size func_0029B16C, . - func_0029B16C
