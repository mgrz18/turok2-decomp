/* 0x00293EBC-0x00293EC0: 1 words splat took for the start of a
 * function. Never executed: lui $11,0x800b.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293EBC
.type func_00293EBC, @object
func_00293EBC:
    .word 0x3C0B800B
.size func_00293EBC, . - func_00293EBC
