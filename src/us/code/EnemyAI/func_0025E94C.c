#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"

void EnemyAI_DrawFadeOut();
void CGameRegion__FireTriggers(struct CGameRegion *region, s32 flags, CGameObjectInstance *obj);

void EnemyMode_DeathMelt_Enter(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = EnemyAI_DrawFadeOut;
    ai->m_dwFlags |= 0x20;
    obj->m_dwHotPointFlags = 0;
    obj->m_dwFlags &= ~0x01000000;
    obj->m_dwFlags &= ~0x00010000;
    AI_StopSound(obj, ai);
    CGameRegion__FireTriggers(obj->header.m_pCurrentRegion, 0x80, obj);
}
