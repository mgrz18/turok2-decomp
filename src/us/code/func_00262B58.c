#include "common.h"
#include "structs/game_object.h"

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define CMtxF__RotateX CMtxF__RotateX_i
#endif
s32 CMtxF__RotateX(s32, s32);                    /* extern */
s32 CMtxF__RotateZ(sptr, f32);                    /* extern */

/* Effect callback context: the actor it runs for and the current value. */
#ifdef TARGET_PORT
/* The port (round 13 b5): the context is CGameObjectInstance__PoseBone's bone-posing context
 * (the m_AI.m_pfEffects callback), whose host layout is T2PoseContext (structs/pose_host.h): the
 * N64 view read the bone index at 0x94 (host T2PoseContext.bone is at 0xE0), so the turret's gun
 * and turn nodes were never rotated (n64viewcheck: the a5 sweep's unresolved Turret_Effects). */
#include "structs/pose_host.h"
typedef struct {
    union {
        struct { CGameObjectInstance *owner; };
        struct { u8 _hp_value[__builtin_offsetof(T2PoseContext, bone)]; s32 value; };
    };
} Effect00262B58;
_Static_assert(__builtin_offsetof(Effect00262B58, owner) == __builtin_offsetof(T2PoseContext, obj), "pose ctx");
#else
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 value;
} Effect00262B58;
#endif

void Turret_Effects(sptr arg0, Effect00262B58 *arg1) {
    CGameObjectInstance *temp_a0;
    CTurretIntelligence *temp_s1;
    CGameObjectInstance *temp_v0;

    temp_a0 = arg1->owner;
    temp_s1 = &temp_a0->header.m_pIntelligence->m_Turret;
    if (arg1->value == temp_s1->m_GunNode) {
#ifdef TARGET_PORT
        /* the gun's pitch: the float whose bits m_VarBuffer[0] holds (MIPS passes them in $a1); the
         * s32 shim CMtxF__RotateX_i would cut the matrix pointer to 32 bits (round 13 b5: reached
         * once the bone index above is read right) */
#undef CMtxF__RotateX
        {
            void CMtxF__RotateX(void *m, f32 a);
            f32 pitch;
            s32 bits = temp_a0->m_AI.m_VarBuffer[0];
            __builtin_memcpy(&pitch, &bits, sizeof(pitch));
            CMtxF__RotateX((void *) arg0, pitch);
        }
#else
        CMtxF__RotateX(arg0, temp_a0->m_AI.m_VarBuffer[0]);
#endif
    }
    if (arg1->value == temp_s1->m_RotNode) {
        temp_v0 = arg1->owner;
        CMtxF__RotateZ(arg0, temp_v0->m_AI.m_InitialRotY - temp_v0->m_RotY);
    }
}
