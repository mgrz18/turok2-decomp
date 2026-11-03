/* 0x004567AC-0x004567B0: 1 words splat took for the start of a
 * function. Never executed: junk between objects; callers jal 0x41D7B0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_004567AC
.type func_004567AC, @object
func_004567AC:
    .word 0x45000007
.size func_004567AC, . - func_004567AC
