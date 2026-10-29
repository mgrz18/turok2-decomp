/* 0x0029B5AC-0x0029B5B0: 1 words splat took for the start of a
 * function. Never executed: a jal whose delay slot is osJamMesg's first word.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B5AC
.type func_0029B5AC, @object
func_0029B5AC:
    .word 0x0C083BCB
.size func_0029B5AC, . - func_0029B5AC
