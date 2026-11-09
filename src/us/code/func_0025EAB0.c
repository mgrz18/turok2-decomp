#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 EnemyAI_UsesRideTables(CGameObjectInstance *arg0, CAI *arg1) {
    sptr temp_v1;

    temp_v1 = (sptr) arg1->m_pModeTable;
    if ((temp_v1 == (sptr) gEnemyStationaryGroundModeTable) || (temp_v1 == (sptr) gEnemyStationaryAirModeTable) || (temp_v1 == (sptr) gEnemyStationaryUnderwaterModeTable)) {
        return 0;
    }
    return (u16) arg0->m_nTypeFlag != 0x259;
}
