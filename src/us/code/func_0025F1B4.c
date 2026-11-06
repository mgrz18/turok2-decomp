#include "common.h"

s32 EnemyMode_Move_Update();                            /* extern */

void EnemyMode_TakeCover_Update(void) {
    EnemyMode_Move_Update();
}
