/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_14.c, func_8024CB4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char *CISet__GetBlock(s32 *, s32);
extern void CQuaternion__Slerp(void *arg0, s32 arg1, void *arg2, void *arg3);
void Anim_GetBoneRotationSlerp(ObjectLinks24_2 *arg0, s32 arg1, Vector4f *arg2) {
    ObjectLinks24_2 *o = arg0;
    ObjRecAt2V1 *indices;
    s16 idx;
    ObjectState14 *rec;
    void *base;
    f32 scale;
    indices = (void *)o->unk_0;
    idx = indices[arg1].unk2;
    if (idx == -1) {
        rec = (void *)(o->unk_4 + arg1 * 0x14);
        scale = 3.051851e-5f;
        arg2->x = (f32)rec->unk_C * scale;
        arg2->y = (f32)rec->unk_E * scale;
        arg2->z = (f32)rec->unk_10 * scale;
        arg2->w = (f32)rec->unk_12 * scale;
        return;
    }
    base = CISet__GetBlock(o->unk_C, (s32)idx);
    CQuaternion__Slerp(arg2, o->unk_20,
                    (char *)base + o->unk_10 * 4,
                    (char *)base + o->unk_14 * 4);
}
