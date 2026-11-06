#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C33C.c, func_0024C33C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B11Box B11Box;
void CAnimationState__GetBonePosition(f32 *p, s32 a, f32 t, f32 *out);
void CMtxF__SetRotateY(f32 *m, s32 a);
void CMtxF__TransformPoint(f32 *m, f32 *v, f32 *out);
void GameObject_TraceMove(CGameObjectInstance *a, CVector3 v, CVector3 *out, B11Box **box, s32 e, s32 f);
void func_0021F668(CGameObjectInstance *a, f32 *p) {
    f32 t0[3];
    f32 t1[3];
    f32 d[3];
    f32 r[3];
    CVector3 w;
    f32 m[16];
    CAnimationState__GetBonePosition(p, 0, 0.0f, t0);
    CAnimationState__GetBonePosition(p, 0, *p, t1);
    d[0] = t1[0] - t0[0];
    d[1] = 0.0f;
    d[2] = t1[1] - t0[1];
    CMtxF__SetRotateY(m, *(s32 *)&a->m_RotY);
    CMtxF__TransformPoint(m, d, r);
    w.x = a->header.m_vPos.x + r[0] * a->m_vScale.x;
    w.y = a->header.m_vPos.y;
    w.z = a->header.m_vPos.z + r[2] * a->m_vScale.z;
    GameObject_TraceMove(a, w, &a->header.m_vPos, (B11Box **)&a->header.m_pCurrentRegion, 0, 0);
}
