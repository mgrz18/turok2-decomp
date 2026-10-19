/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025FFB0_de.c, func_8025FFB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800E679C;
extern float func_00217AD4(float value, int bits);
extern s32 D_800E6798;
extern s32 D_800E67A0;
extern u32 func_00217C58(u32 *arg0, s32 arg1);
extern f32 func_00217AF0(f32 arg0, f32 arg1, s32 arg2);
extern void func_00217214(Curve *arg0);
void func_00217450(Segment8025FFD0 *arg0, u32 *arg1) {
    u32 bits;
    u32 first;
    u32 second;
    f32 lo;
    f32 hi;
    bits = *arg1;
    arg0->unk2C = func_00217C58(&bits, 6);
    if (D_800E6798 != 0) {
        first = func_00217C58(&bits, 0x20);
        second = func_00217C58(&bits, 0x20);
        lo = *(f32 *)&first;
        hi = *(f32 *)&second;
    } else {
        first = func_00217C58(&bits, 6);
        second = func_00217C58(&bits, 6);
        lo = func_00217AD4((f32)first, 6);
        hi = func_00217AD4((f32)second, 6);
        lo = func_00217AF0(lo, D_800E679C, D_800E67A0);
        hi = func_00217AF0(hi, D_800E679C, D_800E67A0);
    }
    arg0->unk4 = lo;
    arg0->unk8 = hi - lo;
    do {
        arg0->unk0 = func_00217C58(&bits, 6);
        arg0->unk28 = 0;
        arg0->unk24 = 0;
        arg0->unkC = bits;
        func_00217214(arg0);
    } while (0);
    *arg1 = bits;
}
