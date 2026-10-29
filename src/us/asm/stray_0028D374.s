/* 0x0028D374-0x0028D380: 3 words splat took for the start of a
 * function. Never executed: jal func_0021E03C, swc1, addu; the boot stub enters at 0x28D380 (main).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028D374
.type func_0028D374, @object
func_0028D374:
    .word 0x0C08780F
    .word 0xE7A00028
    .word 0x00002021
.size func_0028D374, . - func_0028D374
