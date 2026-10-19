/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_12.c, func_8024C92C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0021FA2C(void *arg0, s32 arg1, void *arg2);
extern char *func_002017D4(s32 *, s32);
void func_0021FA2C(void *arg0, s32 arg1, void *arg2) {
    ObjectLinks24 *o = arg0;
    Vec3 *out = arg2;
    ObjRecAt2V1 *indices = (void *)o->unk_0;
    s16 idx;
    ObjectState14 *recs;
    f32 *base;
    f32 *a;
    f32 scale;
    idx = indices[arg1].translation;
    if (idx == -1) {
        recs = (void *)o->unk_4;
        *out = recs[arg1].position;
        return;
    }
    base = func_002017D4(o->translationFrames, (s32) idx);
    a = base + o->translationFirst;
    base = base + o->translationNext;
    scale = o->unk_20;
    out->x = a[0] + scale * (base[0] - a[0]);
    out->y = a[1] + scale * (base[1] - a[1]);
    out->z = a[2] + scale * (base[2] - a[2]);
}
