/* 0x0042EB4C-0x0042EB50: 1 words splat took for the start of a
 * function. Never executed: linker fill before the 16-aligned func_0042EB50.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042EB4C
.type func_0042EB4C, @object
func_0042EB4C:
    .word 0x46016300
.size func_0042EB4C, . - func_0042EB4C
