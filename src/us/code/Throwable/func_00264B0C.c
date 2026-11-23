#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

extern void CMtxF__FromMtxAffine(f32 *out, f32 *mtx);
extern void CMtxF__SetTranslate(f32 *mtx, f32 x, f32 y, f32 z);
extern void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
extern void CMtxF__TransformPoint(f32 *mtx, f32 *in, f32 *out);
extern f32 GameObject_GetCollisionHeight(void *obj);

#ifdef TARGET_PORT
/* TARGET_PORT: the throwable's vars in CAI::m_VarBuffer. On the N64: the owner at 0x10C
 * ([0]), the tilt angle at 0x110 ([1]), its heading at 0x114 ([2]). The host owner is
 * pointer-wide (m_VarPtr[0], [0..1]), so the tilt moves to m_VarF32[2]/[3] (STABILITY.md S14). */
typedef union {
    struct { u8 _h_pos[__builtin_offsetof(CAI, m_vDesiredPos)]; f32 pos[3]; };
    struct { u8 _h_blend[__builtin_offsetof(CAI, m_ConditionTimer)]; f32 blend; };
    struct { u8 _h_owner[__builtin_offsetof(CAI, m_VarPtr[0])]; CGameObjectInstance *owner; };
    struct { u8 _h_tilt[__builtin_offsetof(CAI, m_VarF32[2])]; f32 tiltAngle; };
    struct { u8 _h_head[__builtin_offsetof(CAI, m_VarF32[3])]; f32 tiltHeading; };
} OwnedState;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ CGameObjectInstance *owner;
} OwnedState;
#endif

/* Midpoint of two attachment points on the owner, lifted by the height. */
void Throwable_GetCarryPoint(void *obj, OwnedState *state, f32 *out) {
    f32 mtxA[16];
    f32 mtxB[16];
    f32 a[4];
    f32 b[4];
    JointPoint *pa = &sThrowableCarryJointA;
    JointPoint *pb = &sThrowableCarryJointB;
    CGameObjectInstance *owner = state->owner;
    CGameObjectInstance *src = owner;

    if (owner->m_pmtDrawMtxs != NULL) {
        CMtxF__FromMtxAffine(mtxA, (f32 *) ((u8 *) owner->m_pmtDrawMtxs + (pa->joint << 6)));
        CMtxF__FromMtxAffine(mtxB, (f32 *) ((u8 *) owner->m_pmtDrawMtxs + (pb->joint << 6)));
    } else {
        CMtxF__SetTranslate(mtxA, src->header.m_vPos.x, src->header.m_vPos.y, src->header.m_vPos.z);
        CMtxF__SetTranslate(mtxB, src->header.m_vPos.x, src->header.m_vPos.y, src->header.m_vPos.z);
    }
    CMtxF__Translate(mtxA, 0.0f, GameObject_GetCollisionHeight(obj), 0.0f);
    CMtxF__Translate(mtxB, 0.0f, GameObject_GetCollisionHeight(obj), 0.0f);
    CMtxF__TransformPoint(mtxA, pa->offset, a);
    CMtxF__TransformPoint(mtxB, pb->offset, b);
    out[0] = (a[0] + b[0]) * 0.5f;
    out[1] = (a[1] + b[1]) * 0.5f;
    out[2] = (a[2] + b[2]) * 0.5f;
}
