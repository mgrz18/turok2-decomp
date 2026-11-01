/* 0x0045E674-0x0045E680: 3 words splat took for the start of a
 * function. Never executed: linker fill after the hand-written func_0045E630.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0045E674
.type func_0045E674, @object
func_0045E674:
    .word 0x10400005
    .word 0x24020002
    .word 0x50620071
.size func_0045E674, . - func_0045E674
