/* 0x00206984-0x00206990: 3 words splat took for the start of a
 * function. Never executed: "m_YScale|"; func_0020793C and func_0020798C pass 0x206990 as a load callback.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00206984
.type func_00206984, @object
func_00206984:
    .word 0x6D5F5953
    .word 0x63616C65
    .word 0x7C000000
.size func_00206984, . - func_00206984
