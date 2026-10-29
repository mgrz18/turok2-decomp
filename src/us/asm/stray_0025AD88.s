/* 0x0025AD88-0x0025AD90: 2 words splat took for the start of a
 * function. Never executed: sw, bnez; callers jal 0x25AD90.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0025AD88
.type func_0025AD88, @object
func_0025AD88:
    .word 0xAFA80020
    .word 0x1440013D
.size func_0025AD88, . - func_0025AD88
