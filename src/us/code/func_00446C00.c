#include "common.h"
#include "structs/game_object.h"
#include "rw/abu_protos.h"

f32 Random_Blend(f32 arg0, f32 arg1);

extern f32 D_800C0900;
extern f32 D_800C0904;

void BlindOne_BossMode3Enter_2(s32 arg0, CAI *arg1) {
    arg1->m_ConditionTimer = Random_Blend(D_800C0900, D_800C0904);
}
