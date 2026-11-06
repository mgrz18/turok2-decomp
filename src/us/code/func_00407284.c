#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

void Primagen_EnterMode1(CGameObjectInstance *obj) {
    if (gLevelScript_bossState.mode == 5) {
        gLevelScript_bossState.state = 5;
    } else if (gLevelScript_bossState.mode == 6) {
        gLevelScript_bossState.state = 6;
    } else if (gLevelScript_bossState.mode == 10) {
        gLevelScript_bossState.state = 10;
    } else {
        gLevelScript_bossState.state = 6;
    }
    obj->m_dwFlags |= 0x2000;
}
