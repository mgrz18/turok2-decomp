/* 0x0029DBC4-0x0029DBD0: 3 words splat took for the start of a
 * function. Never executed: mfc1 $5,$f20; mfc1 $7,$f20; jal 210318.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DBC4
.type func_0029DBC4, @object
func_0029DBC4:
    .word 0x4405A000
    .word 0x4407A000
    .word 0x0C0840C6
.size func_0029DBC4, . - func_0029DBC4
