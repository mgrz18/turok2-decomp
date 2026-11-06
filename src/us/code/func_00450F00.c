#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 PlayerSetupMenu_StepHandicap_2(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_s0;

    temp_s0 = ((CPlayer *) arg1->player)->m_pOptions;
    temp_s0->m_Handicap = Menu_StepValue((s32) arg1, (s32) temp_s0->m_Handicap, 1, 0, 0xA, 0);
    return 0;
}
