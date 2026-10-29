/* 0x0029ACD4-0x0029ACE0: 3 words splat took for the start of a
 * function. Never executed: lui $1,0x800a; lwc1 $f14,31520($1); mfc1 $6,$f20.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029ACD4
.type func_0029ACD4, @object
func_0029ACD4:
    .word 0x3C01800A
    .word 0xC42E7B20
    .word 0x4406A000
.size func_0029ACD4, . - func_0029ACD4
