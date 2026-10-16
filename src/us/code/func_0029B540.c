/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_14.c, func_802BB0F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern int func_0029B540(unsigned int devAddr, unsigned int data);
extern int __osSiDeviceBusy();
extern void func_0029B020(void *arg0, void *arg1, s32 arg2);
extern void func_0029B020(void *arg0, void *arg1, s32 arg2);
extern s32 D_800AA920;
extern s32 D_800AA924;
int func_0029B540(u32 arg0, u32 arg1) {
    if (arg0 & 3) {
        func_0029B020(&D_800AA920, &D_800AA924, 0x34);
    }
    if (__osSiDeviceBusy() != 0) {
        return -1;
    }
    *(u32 *)(arg0 | 0xA0000000) = arg1;
    return 0;
}
