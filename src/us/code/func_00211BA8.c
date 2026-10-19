/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_80274BF4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00211BA8(void *arg0);
extern char D_800A56C8;
f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern char D_800A56C8;
extern float func_00298470(float);
void func_00211BA8(void *arg0)
{
    ObjRecAt2CV1 *o = arg0;
    f32 magSq;
    f32 mag;
    f32 zOut;
    magSq = (o->unk18 * o->unk18) +
            (o->unk20 * o->unk20);
    zOut = 0.0f;
    if (magSq != 0.0f) {
        mag = func_00298470(magSq);
        magSq = *(f32 *)&D_800A56C8 / mag;
        o->unk24 = o->unk18 * magSq;
        o->unk28 = o->unk1C * magSq;
        zOut = o->unk20 * magSq;
    } else {
        o->unk24 = 0.0f;
        o->unk28 = 0.0f;
    }
    o->unk2C = zOut;
}
