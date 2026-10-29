/* 0x0029DDF8-0x0029DE00: 2 words splat took for the start of a
 * function. Never executed: move $17,$5; sw $31,44($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DDF8
.type func_0029DDF8, @object
func_0029DDF8:
    .word 0x00A08821
    .word 0xAFBF002C
.size func_0029DDF8, . - func_0029DDF8
