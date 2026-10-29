/* 0x0029A50C-0x0029A510: 1 words splat took for the start of a
 * function. Never executed: lw $2,4($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A50C
.type func_0029A50C, @object
func_0029A50C:
    .word 0x8E020004
.size func_0029A50C, . - func_0029A50C
