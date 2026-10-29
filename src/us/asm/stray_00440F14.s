/* 0x00440F14-0x00440F20: 3 words splat took for the start of a
 * function. Never executed: li $2,1; sh $2,2568($18); lw $2,2640($18).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00440F14
.type func_00440F14, @object
func_00440F14:
    .word 0x24020001
    .word 0xA6420A08
    .word 0x8E420A50
.size func_00440F14, . - func_00440F14
