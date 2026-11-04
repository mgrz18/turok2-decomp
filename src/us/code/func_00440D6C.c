#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(CPlayerList *);                     /* extern */
s32 func_00284204();                                /* extern */

void func_00440D6C(void) {
    CPlayer *temp_v0;

    if (func_00284204() != 0) {
        temp_v0 = func_002532A8(&D_80119870.m_PlayerList);
        if (temp_v0 != NULL) {
            temp_v0->m_Data.m_bPrimagenBossComplete = 1;
            func_00248BA8((s32) temp_v0, 0x1F40, 2, 0);
        }
    }
}
