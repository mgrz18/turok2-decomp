/* 0x00246BBC-0x00246BC0: 1 words splat took for the start of a
 * function. Never executed: a jal; callers jal 0x246BC0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00246BBC
.type func_00246BBC, @object
func_00246BBC:
    .word 0x0C0A60BC
.size func_00246BBC, . - func_00246BBC
