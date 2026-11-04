#include "common.h"
#include "structs/player.h"

/* The menu this handler belongs to: only its player here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} PlayerMenu;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00450F00(s32 arg0, PlayerMenu *arg1) {
    struct CPlayerOptions *temp_s0;

    temp_s0 = arg1->player->m_pOptions;
    temp_s0->m_Handicap = func_0041648C((s32) arg1, (s32) temp_s0->m_Handicap, 1, 0, 0xA, 0);
    return 0;
}
