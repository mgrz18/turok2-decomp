#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} MenuItem;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00417F50(s32 arg0, MenuItem *arg1) {
    CPlayerOptions *temp_s0;

    temp_s0 = (CPlayerOptions *) arg1->player->m_pOptions;
    temp_s0->m_Handicap = func_0041648C((s32) arg1, (s32) temp_s0->m_Handicap, 1, 0, 0xA, 0);
    return 0;
}
