#include "common.h"
#include "structs/player.h"

s32 CPlayer__SetState(s32, s32, s32);
s32 CPlayer__LoseLife(s32);

extern s32 D_800ACDE4;
extern s32 D_800AE340;
extern CPlayer *D_800F1FD0;

void PlayerDeath_CinemaDone(void) {
    if (D_800F1FD0->m_Data.m_Lives == 1) {
        CPlayer__LoseLife((s32) D_800F1FD0);
    } else {
        CPlayer__SetState((s32) D_800F1FD0, (s32) D_800F1FD0, 0x1F);
    }
    D_800ACDE4 = 1;
    D_800AE340 = 0;
}
