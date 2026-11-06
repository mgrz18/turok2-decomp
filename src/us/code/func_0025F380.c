#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

void EnemyMode_PickupThrowObject_Enter(s32 arg0, CAI *arg1) {
    s32 temp_a0;

    temp_a0 = arg1->m_VarBuffer[2];
    CAI__SetMode(temp_a0, temp_a0 + 0x140, 1);
}
