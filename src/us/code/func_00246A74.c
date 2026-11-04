#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002174E4.c, func_002174E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct VecSrc {
    u8 pad0[4]; /* pos at 0x4 */
    CVector3 v;
} VecSrc;
typedef struct VecDst {
               s32 unk0;
               s32 unk4;
               s32 unk8;
               CVector3 a;
               CVector3 b;
               s32 unk24;
               CVector3 c;
               CVector3 d;
               s32 unk40;
} VecDst;
void func_00246A74(VecSrc *src, s32 unused, VecDst *dst) {
    CVector3 a;
    CVector3 b;
    a = src->v;
    b.x = 0;
    b.y = 0;
    b.z = 0;
    dst->unk0 = 3;
    dst->unk4 = 0;
    dst->unk8 = 0;
    dst->a = a;
    dst->b = b;
    dst->unk24 = 0;
    b.y = 0;
    a.y = 0;
    dst->c = a;
    dst->d = b;
    dst->unk40 = 0;
}
