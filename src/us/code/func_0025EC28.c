#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

extern u8 D_800F7078[];
void EnemyAI_DrawFaded();
void EnemyAI_CalcOrientationMatrix();
void CScene__SetInstanceObjectType(void *level, CGameObjectInstance *obj, s32 a, s32 animType);
void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void EnemyAI_Dismount(CGameObjectInstance *obj, CAI *ai, CPlayer *player) {
    obj->m_AI.m_VarBuffer[2] = 0;
    obj->m_dwFlags |= 0x20000;
    obj->m_dwFlags |= 0x10000;
    ai->m_pfDraw = EnemyAI_DrawFaded;
    ai->m_pfCalculateOrientationMatrix = EnemyAI_CalcOrientationMatrix;
    obj->m_vScale = player->m_vOnEnemyOriginalScale;
    obj->m_dwFlags |= 0x20000000;
    CAI__SetMode(obj, ai, 10);
    CScene__SetInstanceObjectType(D_800F7078, obj, player->m_OnEnemyObjectType, ai->m_AnimType);
}
