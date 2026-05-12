/*
 * src/us/boot.c — intentionally empty.
 *
 * The boot routine (ROM 0x1000 / VRAM 0x80000400) is hand-written
 * assembly in src/us/asm/boot.s, wired into the build via the
 * `hasm` subsegment in versions/turok2.us.yaml. It uses COP0
 * registers + tlbwi and is not expressible in SN64-era C.
 *
 * Real C functions for this translation unit will land here once
 * we start matching non-boot code.
 */
