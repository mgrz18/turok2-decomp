/* 0x0029DA28-0x0029DA30: 2 words splat took for the start of a
 * function. Never executed: mul.s, mfc1 after __osDevMgrMain's last j; also in Rage Wars.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DA28
.type func_0029DA28, @object
func_0029DA28:
    .word 0x46150002
    .word 0x4405A000
.size func_0029DA28, . - func_0029DA28
