/* 0x0029F188-0x0029F190: 2 words splat took for the start of a
 * function. Never executed: slti $2,$3,1613; beqz $2,29f1a4.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029F188
.type func_0029F188, @object
func_0029F188:
    .word 0x2862064D
    .word 0x10400005
.size func_0029F188, . - func_0029F188
