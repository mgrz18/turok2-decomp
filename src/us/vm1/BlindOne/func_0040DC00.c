#include "common.h"
#include "rw/abu_protos.h"
#include "structs/game_object.h"

f32 Random_Blend(f32 arg0, f32 arg1);

void BlindOne_BossMode3Enter(CGameObjectInstance *obj, CAI *ai) {
    ai->m_ConditionTimer = Random_Blend(15.0f, 75.0f);
}
