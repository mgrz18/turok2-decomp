#include "common.h"
#include "structs/game.h"

s32 func_0025359C(CPlayerPool *);                   /* extern */
s32 RumblePool_FreeAll(s32 *);                   /* extern */
extern s32 D_800F5A50;

void func_00289FE4(void) {
    RumblePool_FreeAll(&D_800F5A50);
    func_0025359C(&D_80119870.m_PlayerList);
}
