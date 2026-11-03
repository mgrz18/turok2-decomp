/* 0x002150B8-0x002150C0: 2 words splat took for the start of a
 * function. Never executed: "ICKUP_SA"; func_00220D00 calls 0x2150C0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002150B8
.type func_002150B8, @object
func_002150B8:
    .word 0x49434B55
    .word 0x505F5341
.size func_002150B8, . - func_002150B8
