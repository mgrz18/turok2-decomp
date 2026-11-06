#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

void Primagen_EnterMode20(CGameObjectInstance *obj) {
    obj->m_dwFlags |= 0x2000;
    gLevelScript_bossMode.swayTimer = 15.0f;
    gLevelScript_bossMode.timer = 11.25f;
}
