#include "common.h"
#include "structs/game_object.h"

extern s32 gLevelScript_bossState;

void Primagen_EnterMode6(CGameObjectInstance *arg0) {
    s32 *flags;

    /* through a pointer: a plain struct store gets scheduled after gLevelScript_bossState's */
    flags = &arg0->m_dwFlags;
    *flags = (s32) (*flags | 0x2000);
    gLevelScript_bossState = 0;
}
