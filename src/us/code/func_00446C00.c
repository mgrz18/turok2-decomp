#include "common.h"
#include "structs/game_object.h"
#include "rw/abu_protos.h"

f32 Random_Blend(f32 arg0, f32 arg1);

void BlindOne_BossMode3Enter_2(s32 arg0, CAI *arg1) {
    arg1->m_ConditionTimer = Random_Blend(15.0f, 75.0f);
}
