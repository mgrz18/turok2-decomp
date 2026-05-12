/*
 * src/us/boot.c — placeholder for boot-region C code.
 *
 * STATUS: the actual boot routine (ROM 0x1000 / VRAM 0x80000400) is
 * NOT decompilable to C. The function reads/writes COP0 registers
 * (Index, EntryHi, EntryLo0/1, PageMask), issues `tlbwi`, and sets
 * up $sp before any prologue — none of which the SN64 cc1 can emit
 * as a regular function. Even turok3's decomp keeps it as `.s`
 * (references/turok3/src/common/asm/SNMAIN.s).
 *
 * The byte-exact source-of-truth therefore lives in `src/us/boot.s`,
 * hand-written assembly that is verified byte-identical against
 * baserom.us.z64 bytes 0x1000..0x10FC by
 * `scripts/verify_boot_asm.sh` — run it any time `src/us/boot.s`
 * changes.
 *
 * Activation note (for the next agent):
 *   To actually link `src/us/boot.s` into the ROM, either:
 *     (a) remove the boot_main + STVM_MapTLB range from the splat
 *         `asm` segment in versions/turok2.us.yaml so 1000.s no
 *         longer covers it, then add boot.s to the Makefile S_FILES
 *         enumeration, OR
 *     (b) move src/us/boot.s into src/us/asm/ (so the existing
 *         SRC_ASM_DIR glob in Makefile picks it up) AND delete the
 *         overlapping symbols from us/asm/1000.s after each splat
 *         run, OR
 *     (c) replace `versions/turok2.us.yaml`'s 0x1000 asm chunk with
 *         a `c` segment pointing at this directory.
 *   Each requires coordinated edits in files owned by sibling
 *   agents (Makefile / versions/*.yaml), so this commit leaves the
 *   boot.s file as a verified-but-detached reference.
 *
 * Real C functions for this translation unit will land here once
 * we start matching non-boot code (libultra init, main(), etc.).
 */

/* Intentionally empty translation unit. Every prior stub here
 * generated wrong bytes (gcc-style prologue + jal main) and was
 * misleading. The real boot lives in boot.s. */
