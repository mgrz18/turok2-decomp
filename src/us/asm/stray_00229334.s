/* 0x00229334-0x00229340: 3 words splat took for the start of a
 * function. Never executed: .word 0x36000db; dsrl32 $31,$31,0x1f; sd $29,-1($31).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00229334
.type func_00229334, @object
func_00229334:
    .word 0x036000DB
    .word 0x001FFFFE
    .word 0xFFFDFFFF
.size func_00229334, . - func_00229334
