/* 0x0045342C-0x00453430: 1 words splat took for the start of a
 * function. Never executed: a jal never executed; the function starts 16-aligned.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0045342C
.type func_0045342C, @object
func_0045342C:
    .word 0x0C083BCB
.size func_0045342C, . - func_0045342C
