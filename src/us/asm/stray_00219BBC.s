/* 0x00219BBC-0x00219BC0: 1 words splat took for the start of a
 * function. Never executed: "FIRE".
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00219BBC
.type func_00219BBC, @object
func_00219BBC:
    .word 0x46495245
.size func_00219BBC, . - func_00219BBC
