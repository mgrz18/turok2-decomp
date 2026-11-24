#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C33C.c, func_0024C33C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B11Region B11Region;
void CAnimationState__GetBonePosition(void *channel, s32 bone, f32 t, f32 *out);
/* The angle passed as its word: the port's shim (port/game/t2abi.c, ABI.md float_int), the N64
 * definition under its own name. */
#ifdef TARGET_PORT
void CMtxF__SetRotateY_i(void *m, s32 a);
#else
void CMtxF__SetRotateY_i(void *m, s32 a) __asm__("CMtxF__SetRotateY");
#endif
void CMtxF__TransformPoint(void *m, void *in, void *out);
void GameObject_TraceMove(void *actor, CVector3 pos, void *outPos, void *outRegion, s32 flagA, s32 flagB);
void CGameObjectInstance__ApplyRootMotion(CGameObjectInstance *a, f32 *p) {
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
    CMtxF__SetRotateY_i(m, *(s32 *)&a->m_RotY);
    CMtxF__TransformPoint(m, d, r);
    w.x = a->header.m_vPos.x + r[0] * a->m_vScale.x;
    w.y = a->header.m_vPos.y;
    w.z = a->header.m_vPos.z + r[2] * a->m_vScale.z;
    GameObject_TraceMove(a, w, &a->header.m_vPos, (B11Region **)&a->header.m_pCurrentRegion, 0, 0);
}
