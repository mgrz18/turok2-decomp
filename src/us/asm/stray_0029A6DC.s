/* 0x0029A6DC-0x0029A6E0: 1 words splat took for the start of a
 * function. Never executed: sw $17,228($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A6DC
.type func_0029A6DC, @object
func_0029A6DC:
    .word 0xAFB100E4
.size func_0029A6DC, . - func_0029A6DC
