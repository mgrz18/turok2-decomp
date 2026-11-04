#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

extern u8 D_800F7078[];
void func_0025BA80();
void func_0025C018();
void func_002241D8(void *level, CGameObjectInstance *obj, s32 a, s32 animType);
void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

void func_0025EC28(CGameObjectInstance *obj, CAI *ai, CPlayer *player) {
    obj->m_AI.m_VarBuffer[2] = 0;
    obj->m_dwFlags |= 0x20000;
    obj->m_dwFlags |= 0x10000;
    ai->m_pfDraw = func_0025BA80;
    ai->m_pfCalculateOrientationMatrix = func_0025C018;
    obj->m_vScale = player->m_vOnEnemyOriginalScale;
    obj->m_dwFlags |= 0x20000000;
    func_00243414(obj, ai, 10);
    func_002241D8(D_800F7078, obj, player->m_OnEnemyObjectType, ai->m_AnimType);
}
