#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/enemy_ai.h"


void EnemyAI_Mount(CGameObjectInstance *obj, CAI *ai, CPlayer *player) {
    ai->m_Enemy.m_pGrabbed = (CGameObjectInstance *) player; /* pointer-wide (m_VarBuffer[2] is a u32) */
    if (player != NULL) {
        Throwable_SetModeAnim(player, &player->m_CharacterActor.m_AI, obj);
    }
    player->m_vOnEnemyOriginalScale = obj->m_vScale;
    obj->m_dwFlags &= ~0x20000000;
    CScene__SetInstanceObjectType(&gScene, obj, player->m_OnEnemyObjectType + 1, ai->m_AnimType);
    CAI__SetMode(obj, ai, 0x2C);
}
