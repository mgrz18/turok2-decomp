#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_PickupThrowObject_Enter(s32 arg0, CAI *arg1) {
    s32 temp_a0;

    temp_a0 = arg1->m_VarBuffer[2];
    CAI__SetMode(temp_a0, temp_a0 + 0x140, 1);
}
