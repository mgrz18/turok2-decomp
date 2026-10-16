/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_41.c, func_802B9FD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern int __osSpDeviceBusy();
extern s32 func_0029A3A0(s32 direction, s32 spAddress, s32 dramAddress, s32 size);
extern void func_0029B020(void *arg0, void *arg1, s32 arg2);
extern s32 func_0029BF80(s32);
extern s32 D_800AA8B0;
extern s32 D_800AA8B4;
s32 func_0029A3A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 & 7) {
        func_0029B020(&D_800AA8B0, &D_800AA8B4, 0x3A);
    }
    if (arg2 & 7) {
        func_0029B020(&D_800AA8B0, &D_800AA8B4, 0x3B);
    }
    if (arg3 & 7) {
        func_0029B020(&D_800AA8B0, &D_800AA8B4, 0x3C);
    }
    if (__osSpDeviceBusy() != 0) {
        return -1;
    }
    (*(volatile u32 *)(0xA4040000U)) = arg1;
    (*(volatile u32 *)(0xA4040004U)) = func_0029BF80(arg2);
    if (arg0 == 0) {
        (*(volatile u32 *)(0xA404000CU)) = arg3 - 1;
    } else {
        (*(volatile u32 *)(0xA4040008U)) = arg3 - 1;
    }
    return 0;
}
