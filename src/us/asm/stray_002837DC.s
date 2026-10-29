/* 0x002837DC-0x002837E0: 1 words splat took for the start of a
 * function. Never executed: lwc1 $f0,660($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002837DC
.type func_002837DC, @object
func_002837DC:
    .word 0xC6000294
.size func_002837DC, . - func_002837DC
