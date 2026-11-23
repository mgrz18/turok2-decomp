#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

M2C_UNK EnemyAI_CheckOffscreen(sptr, void *, M2C_UNK);        /* extern */

void EnemyAI_SleepIfOffscreen(sptr arg0, void *arg1, M2C_UNK arg2) {
    if ((CScene__GetInstanceSavedBit(&gScene, arg0) == 0) && (((CAI *) arg1)->m_Mode == 0)) {
        EnemyAI_CheckOffscreen(arg0, arg1, arg2);
        CAI__SetMode(arg0, arg1, 2);
    }
}
