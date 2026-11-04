#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

extern u8 D_800F7078[];
void func_00264C8C(CPlayer *player, CAI *ai, CGameObjectInstance *obj);
void func_002241D8(void *level, CGameObjectInstance *obj, s32 a, s32 animType);
void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

void func_0025EB80(CGameObjectInstance *obj, CAI *ai, CPlayer *player) {
    ai->m_VarBuffer[2] = (u32) player;
    if (player != NULL) {
        func_00264C8C(player, &player->m_CharacterActor.m_AI, obj);
    }
    player->m_vOnEnemyOriginalScale = obj->m_vScale;
    obj->m_dwFlags &= ~0x20000000;
    func_002241D8(D_800F7078, obj, player->m_OnEnemyObjectType + 1, ai->m_AnimType);
    func_00243414(obj, ai, 0x2C);
}
