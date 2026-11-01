/* 0x004691B8-0x004691C0: 2 words splat took for the start of a
 * function. Never executed: sw v0,0(t3); move a1,zero: junk between objects.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_004691B8
.type func_004691B8, @object
func_004691B8:
    .word 0xAD620000
    .word 0x00002821
.size func_004691B8, . - func_004691B8
