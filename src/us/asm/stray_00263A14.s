/* 0x00263A14-0x00263A20: 3 words splat took for the start of a
 * function. Never executed: addu, addu, jal func_0020EEF8; func_00264378 calls 0x263A20.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00263A14
.type func_00263A14, @object
func_00263A14:
    .word 0x02002021
    .word 0x02002821
    .word 0x0C083BBE
.size func_00263A14, . - func_00263A14
