#include "common.h"
#include "structs/game_object.h"


typedef struct EventParams {
    s32 unk0;           /* 0x0 */
    f32 size;           /* 0x4 */
} EventParams;

extern u8 gScene_m_InstancePool[];
extern void CInstancePool__SpawnAnimInstance(void *arg0, s32 arg1, s32 arg2, CVector3 pos, s32 arg6, CVector3 rot, CVector3 scale, s32 arg13);

void ImpactEvent_SpawnAnimInstance(CGameObjectInstance *actor, s32 arg1, s32 arg2, CVector3 arg3, EventParams params) {
    CVector3 scale;
    CVector3 rot;

    if (actor->header.m_Type == 0) {
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
    } else {
        rot = actor->m_vVelocity;
    }
    scale.x = params.size;
    scale.y = params.size;
    scale.z = params.size;
    CInstancePool__SpawnAnimInstance(gScene_m_InstancePool, params.unk0, ((s32)actor->header.m_pCurrentRegion), actor->header.m_vPos, 0, rot, scale, 0);
}
