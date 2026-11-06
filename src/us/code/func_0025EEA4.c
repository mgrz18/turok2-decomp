#include "common.h"
#include "m2c_macros.h"

s32 CScene__GetInstanceSavedBit(M2C_UNK *, s32);                  /* extern */
M2C_UNK CAI__SetMode(s32, void *, M2C_UNK);        /* extern */
M2C_UNK EnemyAI_CheckOffscreen(s32, void *, M2C_UNK);        /* extern */
extern M2C_UNK gScene;

void EnemyAI_SleepIfOffscreen(s32 arg0, void *arg1, M2C_UNK arg2) {
    if ((CScene__GetInstanceSavedBit(&gScene, arg0) == 0) && (M2C_FIELD(arg1, s8 *, 0x34) == 0)) {
        EnemyAI_CheckOffscreen(arg0, arg1, arg2);
        CAI__SetMode(arg0, arg1, 2);
    }
}
