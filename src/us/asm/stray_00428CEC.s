/* 0x00428CEC-0x00428CF0: 1 words splat took for the start of a
 * function. Never executed: jal 268b48.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00428CEC
.type func_00428CEC, @object
func_00428CEC:
    .word 0x0C09A2D2
.size func_00428CEC, . - func_00428CEC
