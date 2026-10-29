/* 0x00294644-0x00294650: 3 words splat took for the start of a
 * function. Never executed: bnel into alSynStopVoice.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00294644
.type func_00294644, @object
func_00294644:
    .word 0x00621021
    .word 0x284200FF
    .word 0x54400003
.size func_00294644, . - func_00294644
