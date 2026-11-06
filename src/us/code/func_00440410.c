#include "common.h"
#include "structs/game_object.h"

extern s32 gLevelScript_bossState;

void Primagen_EnterMode6_2(CGameObjectInstance *arg0) {
    s32 *flags = &arg0->m_dwFlags;

    *flags |= 0x2000;
    gLevelScript_bossState = 0;
}
