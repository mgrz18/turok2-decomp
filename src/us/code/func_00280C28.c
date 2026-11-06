/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00238EA8.c, func_00238EA8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern s32 gDefaultVisBits[]; /* local type: the u32 of globals/camera.h loads it first (GCC 2 alias order) */
void CCameraViewParams__Construct(ObjInit *o) {
    CVector3 *p;
    f32 z;
    o->unk20.i = 0;
    z = o->unk20.f;
    o->unk0 = 1;
    o->unk4 = 0;
    o->unk8 = 0;
    o->unkC = 0;
    o->unk10 = 0;
    o->unk14 = 0;
    o->unk18 = 0;
    o->unk1C = 0;
    p = &o->v;
    p->x = p->y = p->z = z;
    o->unk30 = 1.0f;
    o->unk34 = 0;
    o->unk38 = z;
    o->unk3C = 0.17453295f;
    o->unk40 = gDefaultVisBits[0];
}
