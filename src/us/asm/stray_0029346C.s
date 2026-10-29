/* 0x0029346C-0x00293470: 1 words splat took for the start of a
 * function. Never executed: lw $17,124($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029346C
.type func_0029346C, @object
func_0029346C:
    .word 0x8FB1007C
.size func_0029346C, . - func_0029346C
