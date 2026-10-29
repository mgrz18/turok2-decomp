/* 0x0028EF84-0x0028EF90: 3 words splat took for the start of a
 * function. Never executed: beqz into __alCSeqGetTrackEvent.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028EF84
.type func_0028EF84, @object
func_0028EF84:
    .word 0x2463ACB0
    .word 0x8C620028
    .word 0x10400007
.size func_0028EF84, . - func_0028EF84
