/* 0x00460E98-0x00460EA0: 2 words splat took for the start of a
 * function. Never executed: lhu $2,8($17); beqzl $2,460ea4.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00460E98
.type func_00460E98, @object
func_00460E98:
    .word 0x96220008
    .word 0x50400001
.size func_00460E98, . - func_00460E98
