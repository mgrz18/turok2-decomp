/* 0x00228BA4-0x00228BB0: 3 words splat took for the start of a
 * function. Never executed: a nop and two data words ahead of CSunFrameData__Construct.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00228BA4
.type func_00228BA4, @object
func_00228BA4:
    .word 0x00000000
    .word 0x0001B9D2
    .word 0x00003A98
.size func_00228BA4, . - func_00228BA4
