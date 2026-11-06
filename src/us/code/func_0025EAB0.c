#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 EnemyAI_UsesRideTables(CGameObjectInstance *arg0, CAI *arg1) {
    s32 temp_v1;

    temp_v1 = (s32) arg1->m_pModeTable;
    if ((temp_v1 == (s32) gEnemyStationaryGroundModeTable) || (temp_v1 == (s32) gEnemyStationaryAirModeTable) || (temp_v1 == (s32) gEnemyStationaryUnderwaterModeTable)) {
        return 0;
    }
    return (u16) arg0->m_nTypeFlag != 0x259;
}
