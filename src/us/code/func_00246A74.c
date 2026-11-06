#include "structs/vector.h"
#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002174E4.c, func_002174E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct VecSrc {
    u8 pad0[4]; /* pos at 0x4 */
    CVector3 v;
} VecSrc;
void AI_SetTargetPoint(VecSrc *src, s32 unused, CAITarget *dst) {
    CVector3 a;
    CVector3 b;
    a = src->v;
    b.x = 0;
    b.y = 0;
    b.z = 0;
    dst->m_Kind = 3;
    dst->m_pTarget = 0;
    dst->m_Angle = 0;
    dst->m_vPos = a;
    dst->m_vDelta = b;
    dst->m_Dist = 0;
    b.y = 0;
    a.y = 0;
    dst->m_vFlatPos = a;
    dst->m_vFlatDelta = b;
    dst->m_FlatDist = 0;
}
