/* 0x0028D81C-0x0028D820: 1 words splat took for the start of a
 * function. Never executed: sra $7,$7,0x10.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028D81C
.type func_0028D81C, @object
func_0028D81C:
    .word 0x00073C03
.size func_0028D81C, . - func_0028D81C
