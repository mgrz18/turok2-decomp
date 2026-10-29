/* 0x00228908-0x00228910: 2 words splat took for the start of a
 * function. Never executed: sllv $0,$0,$0; sdc2 $14,-3478($5).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00228908
.type func_00228908, @object
func_00228908:
    .word 0x00000004
    .word 0xF8AEF26A
.size func_00228908, . - func_00228908
