#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00450F00(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_s0;

    temp_s0 = ((CPlayer *) arg1->player)->m_pOptions;
    temp_s0->m_Handicap = func_0041648C((s32) arg1, (s32) temp_s0->m_Handicap, 1, 0, 0xA, 0);
    return 0;
}
