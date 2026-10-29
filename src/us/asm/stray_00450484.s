/* 0x00450484-0x00450490: 3 words splat took for the start of a
 * function. Never executed: lb $2,199($5); beqz $2,450498; nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00450484
.type func_00450484, @object
func_00450484:
    .word 0x80A200C7
    .word 0x10400003
    .word 0x00000000
.size func_00450484, . - func_00450484
