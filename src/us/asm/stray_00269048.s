/* 0x00269048-0x00269050: 2 words splat took for the start of a
 * function. Never executed: and $4,$4,$30; jal 26d128.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00269048
.type func_00269048, @object
func_00269048:
    .word 0x009E2024
    .word 0x0C09B44A
.size func_00269048, . - func_00269048
