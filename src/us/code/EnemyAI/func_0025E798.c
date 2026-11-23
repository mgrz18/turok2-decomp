#include "common.h"
#include "structs/game_object.h"

extern void Sound__Play(s16 sound, CVector3 pos, sptr, sptr);

void EnemyMode_Start_Enter(CGameObjectInstance *obj) {
    u16 temp_a0;

    temp_a0 = obj->header.m_pIntelligence->m_Enemy.m_StartSound;
    if (temp_a0 != 0) {
        Sound__Play((s16) temp_a0, obj->header.m_vPos, 0, -1);
    }
}
