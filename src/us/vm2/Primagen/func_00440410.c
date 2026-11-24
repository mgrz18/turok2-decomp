#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"

void Primagen_EnterMode6_2(CGameObjectInstance *arg0) {
    s32 *flags;

    /* through a pointer: a plain struct store gets scheduled after gLevelScript_bossState's */
    flags = &arg0->m_dwFlags;
    *flags = (s32) (*flags | 0x2000);
    gLevelScript_bossState.state = 0;
}
