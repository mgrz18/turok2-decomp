#include "common.h"
#include "structs/game_object.h"

void AI_StopSound(CGameObjectInstance *obj, CAI *ai);
void CListedInstance__Free(CGameObjectInstance *obj);

void EnemyMode_DeathMelt_Update(CGameObjectInstance *obj, CAI *ai) {
    f32 t;
    f32 squash;

    ai->m_OnFireTimer = 0;
    obj->m_dwHotPointFlags &= ~2;
    AI_StopSound(obj, ai);
    t = obj->m_AI.m_ModeTime;
    if (t > 30.0f) {
        squash = 0.01f;
        obj->m_dwFlags &= ~0x100;
        obj->m_dwFlags &= ~0x2000;
        AI_StopSound(obj, ai);
        if (obj->m_dwFlags & 0x80000) {
            CListedInstance__Free(obj);
        }
    } else {
        squash = t * (0.01f / 30.0f);
    }
    obj->m_vScale.x *= 1.0f + squash;
    obj->m_vScale.y *= 1.0f - squash;
    obj->m_vScale.z *= 1.0f + squash;
}
