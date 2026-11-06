#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00217224.c, func_00217224.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);
typedef struct { u8 pad0[4]; CVector3 pos; u8 pad10[0x44]; f32 unk54; } B04Obj; /* Turok 2: pos 0x4, height 0x54 */
f32 AI_DistSqToPointAtHeight(B04Obj *o, CVector3 v) {
    CVector3__Subtract(&v, &v, &o->pos);
    v.y = v.y - o->unk54;
    return v.x * v.x + v.y * v.y + v.z * v.z;
}
