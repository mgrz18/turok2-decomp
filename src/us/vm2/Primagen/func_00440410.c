#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"

void Primagen_EnterMode6_2(CGameObjectInstance *arg0) {
    s32 *flags = &arg0->m_dwFlags;

    *flags |= 0x2000;
    gLevelScript_bossState.state = 0;
}
