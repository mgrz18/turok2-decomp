/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80405DC0_16.CPP, func_804098CC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern void func_00421334(void ***handle, void **data, s32 units);
extern void * func_00266C5C(void *arg0, int arg1, u32 arg2);
extern char *D_800F70A8;
extern char D_800C13F0[];
extern void func_002053A8(s32 a);
extern void func_002052D8(void *arg0, void *arg1);
extern void **func_00204EDC(s32, s32, s32, char *);
void func_002052D8(void *, void *);
void func_002053A8(s32);
void func_00421334(void ***handle, void **data, s32 units) {
    void **block;
    void *first;
    if (*handle != 0) {
        func_002053A8(0);
        func_002052D8(0, *handle);
    }
    if (units != 0) {
        units <<= 8;
    } else {
        units = (s32)(D_800F70A8 + 0x688);
    }
    block = func_00204EDC(0, units, 0x33, D_800C13F0);
    *handle = block;
    first = *block;
    *data = first;
    func_00266C5C(first, 0, units);
}
