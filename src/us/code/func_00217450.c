/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025FFB0_de.c, func_8025FFB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800E679C;
extern float Bits_Normalize(float value, int bits);
extern s32 D_800E6798;
extern s32 D_800E67A0;
extern u32 Bits_Read(u32 *arg0, s32 arg1);
extern f32 Math_Remap(f32 arg0, f32 arg1, s32 arg2);
extern void AnimCurve_LoadSegment(Curve *arg0);
void AnimCurve_Init(Segment8025FFD0 *arg0, u32 *arg1) {
    u32 bits;
    u32 first;
    u32 second;
    f32 lo;
    f32 hi;
    bits = *arg1;
    arg0->unk2C = Bits_Read(&bits, 6);
    if (D_800E6798 != 0) {
        first = Bits_Read(&bits, 0x20);
        second = Bits_Read(&bits, 0x20);
        lo = *(f32 *)&first;
        hi = *(f32 *)&second;
    } else {
        first = Bits_Read(&bits, 6);
        second = Bits_Read(&bits, 6);
        lo = Bits_Normalize((f32)first, 6);
        hi = Bits_Normalize((f32)second, 6);
        lo = Math_Remap(lo, D_800E679C, D_800E67A0);
        hi = Math_Remap(hi, D_800E679C, D_800E67A0);
    }
    arg0->unk4 = lo;
    arg0->unk8 = hi - lo;
    do {
        arg0->unk0 = Bits_Read(&bits, 6);
        arg0->unk28 = 0;
        arg0->unk24 = 0;
        arg0->unkC = bits;
        AnimCurve_LoadSegment(arg0);
    } while (0);
    *arg1 = bits;
}
