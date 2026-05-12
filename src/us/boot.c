/*
 * src/us/boot.c — candidate hand-written `romMain` for byte-exact matching.
 *
 * This file is currently NOT linked into the rom — `make build` produces a
 * byte-exact baserom by using the splat-emitted us/asm/1000.s. This source
 * lives here so the C-compile pipeline (cpp -> cc1 -> asn64.exe -> psyq-
 * obj-parser) can be exercised end-to-end before we start removing
 * functions from the splat asm.
 *
 * Target sequence at 0x80000400 (from us/asm/1000.s):
 *
 *     romMain:
 *       lui   $sp, 0x803F
 *       ori   $sp, $sp, 0xFFC0       ; sp = 0x803EFFC0
 *       addiu $a0, $zero, 0x1E       ; loop counter for TLB invalidation
 *     ...
 *
 * The full boot routine clears the TLB and jumps to STVM_MapTLB then main.
 * For now this is just an entrypoint stub — it will diff against the
 * original until we get the codegen flags / inline asm right.
 */

extern void STVM_MapTLB(int index, int pagemask, int vaddr, int paddr0, int paddr1, int flags);
extern int main(void);

/*
 * Probably needs to be written almost entirely in inline asm to match the
 * cop0 register manipulation. The C visible part is just the entrypoint.
 */
void romMain(void) {
    /* placeholder — real implementation requires inline asm for mtc0/tlbwi */
    main();
}
