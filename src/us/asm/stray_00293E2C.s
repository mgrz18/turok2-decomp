/* 0x00293E2C-0x00293E30: 1 words splat took for the start of a
 * function. Never executed: beql into alSynAllocVoice.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293E2C
.type func_00293E2C, @object
func_00293E2C:
    .word 0x50440083
.size func_00293E2C, . - func_00293E2C
