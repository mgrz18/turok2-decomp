/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/__osSpSetPc.c, __osSpSetPc.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef signed int s32;
s32 func_0029A4A0(u32 pc);
s32 func_0029A4A0(u32 pc);
s32 func_0029A4A0(u32 pc) {
    if (!((*(volatile u32 *)((u32)(0x04040010) | 0xA0000000)) & 0x0001)) {
        return -1;
    }
    (*(volatile u32 *)((u32)(0x04080000) | 0xA0000000) = (u32)(pc));
    return 0;
}
