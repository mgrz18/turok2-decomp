/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_9.c, func_802604AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0020EF8C(Vec3 *arg0, s32 arg1, void *arg2, void *arg3);
void func_0021794C(void *arg0, s32 arg1, void *arg2) {
    extern void *func_002017D4(void *arg0, s32 arg1);
    ObjectLinks24_2 *o = arg0;
    ObjRecAt2V1 *indices = (void *)o->unk_0;
    s16 idx;
    Rec_func_8024C92C_de *recs;
    void *base;
    Triple *out = arg2;
    idx = indices[arg1].translation;
    if (idx == -1) {
        recs = (void *)o->unk_4;
        __builtin_memcpy(out, &recs[arg1], sizeof(*out));
        return;
    }
    base = func_002017D4(o->unk_8, (s32) idx);
    func_0020EF8C(arg2, o->unk_20,
                  (char *)base + (o->unk_18) * 4,
                  (char *)base + (o->unk_1C) * 4);
}
