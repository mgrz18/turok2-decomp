/* 0x002946CC-0x002946D0: 1 words splat took for the start of a
 * function. Never executed: beqz into alSynNew.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002946CC
.type func_002946CC, @object
func_002946CC:
    .word 0x10400015
.size func_002946CC, . - func_002946CC
