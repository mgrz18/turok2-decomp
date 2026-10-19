/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B758_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A8174;
extern float D_800A8178;
extern s16 D_800F2940[];
extern float D_800A8170;
void func_00273A94(s32 *arg0, s32 arg1) {
    s32 i;
    s32 idx;
    f32 t;
    f32 p;
    s32 off;
    RecordCode8025A3EC *rec;
    f32 k1;
    f32 k2;
    f32 k3;
    s32 negone;
    s32 one;
    i = 0;
    k1 = D_800A8170;
    k2 = D_800A8174;
    *arg0 = arg1;
    do {
        t = (f32)i * k1;
        p = t * t * t * t * t * k2;
        idx = 0x5A - i;
        i += 1;
        D_800F2940[idx] = (s16)(s32)p;
    } while (i < 0x5B);
    negone = -1;
    i = 0;
    k3 = D_800A8178;
    one = 1;
    off = i;
    do {
        rec = (RecordCode8025A3EC *)((u32)off + (u32)arg0);
        rec = &((ObjRecAt4V6 *)(rec))->unk4;
        rec->index = i;
        i += 1;
        rec->owner = arg1;
        rec->f0C = negone;
        rec->f08 = negone;
        rec->f3A = (s16)negone;
        rec->f38 = 0;
        rec->f40 = negone;
        rec->f14 = 0;
        rec->f2C = k3;
        rec->f58 = 0;
        rec->f5C = 0;
        rec->f34 = k3;
        rec->fA4 = 0;
        rec->fAC = 0;
        rec->fB4 = negone;
        rec->fB8 = k3;
        rec->fBC = 0;
        rec->fC0 = 0;
        rec->fC4 = one;
        off += 0xCC;
    } while (i < 0x11);
}
