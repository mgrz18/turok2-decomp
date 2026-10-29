/* 0x00266B7C-0x00266B80: 1 words splat took for the start of a
 * function. Never executed: lw $6,340($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00266B7C
.type func_00266B7C, @object
func_00266B7C:
    .word 0x8FA60154
.size func_00266B7C, . - func_00266B7C
