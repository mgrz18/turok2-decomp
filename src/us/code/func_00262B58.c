#include "common.h"
#include "structs/game_object.h"

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define CMtxF__RotateX CMtxF__RotateX_i
#endif
s32 CMtxF__RotateX(s32, s32);                    /* extern */
s32 CMtxF__RotateZ(s32, f32);                    /* extern */

/* Effect callback context: the actor it runs for and the current value. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 value;
} Effect00262B58;

void Turret_Effects(s32 arg0, Effect00262B58 *arg1) {
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
