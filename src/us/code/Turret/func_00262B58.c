#include "common.h"
#include "structs/game_object.h"

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define CMtxF__RotateX CMtxF__RotateX_i
#endif
s32 CMtxF__RotateX(s32, s32);                    /* extern */
s32 CMtxF__RotateZ(sptr, f32);                    /* extern */

/* Effect callback context: the actor it runs for and the current value. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 value;
} Effect00262B58;

#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* Host body (round 13 b4): CGameObjectInstance__PoseBone passes the pose context at its
 * HOST layout (structs/pose_host.h: obj, bone) and the matrix is a host static
 * (sPoseMtx): the s32 CMtxF__RotateX_i shim truncated it (macOS crash in level 124, the
 * first turret of the whole-game run). The float CMtxF__RotateX directly, the matrix
 * pointer-wide; m_VarBuffer[0] holds the gun pitch as float bits. */
#undef CMtxF__RotateX
void CMtxF__RotateX_f(void *m, f32 a) __asm__("CMtxF__RotateX"); /* the float definition */
void Turret_Effects(sptr arg0, T2PoseContext *ctx) {
    CGameObjectInstance *obj = ctx->obj;
    CTurretIntelligence *turret = &obj->header.m_pIntelligence->m_Turret;
    union {
        u32 u;
        f32 f;
    } pitch;

    if (ctx->bone == turret->m_GunNode) {
        pitch.u = obj->m_AI.m_VarBuffer[0];
        CMtxF__RotateX_f((void *) arg0, pitch.f);
    }
    if (ctx->bone == turret->m_RotNode) {
        CMtxF__RotateZ(arg0, obj->m_AI.m_InitialRotY - obj->m_RotY);
    }
}
#else
void Turret_Effects(sptr arg0, Effect00262B58 *arg1) {
    CGameObjectInstance *temp_a0;
    CTurretIntelligence *temp_s1;
    CGameObjectInstance *temp_v0;

    temp_a0 = arg1->owner;
    temp_s1 = &temp_a0->header.m_pIntelligence->m_Turret;
    if (arg1->value == temp_s1->m_GunNode) {
        CMtxF__RotateX(arg0, temp_a0->m_AI.m_VarBuffer[0]);
    }
    if (arg1->value == temp_s1->m_RotNode) {
        temp_v0 = arg1->owner;
        CMtxF__RotateZ(arg0, temp_v0->m_AI.m_InitialRotY - temp_v0->m_RotY);
    }
}
#endif
