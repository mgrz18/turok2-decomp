/* 0x0028A018-0x0028A020: 2 words splat took for the start of a
 * function. Never executed: lwc1, addu; func_0028ADF0 starts the thread at 0x28A020.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028A018
.type func_0028A018, @object
func_0028A018:
    .word 0xC4206F98
    .word 0x02602021
.size func_0028A018, . - func_0028A018
