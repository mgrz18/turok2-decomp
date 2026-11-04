#include "common.h"
#include "structs/player.h"

/* Multiplayer options kept per player (CPlayer.m_pOptions; see
 * func_00247C10.c): only the byte at 0x32 here. */
struct CPlayerOptions {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u8 unk32;            /* 1..10 */
};

/* The menu this handler belongs to: only its player here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} PlayerMenu;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00450F00(s32 arg0, PlayerMenu *arg1) {
    struct CPlayerOptions *temp_s0;

    temp_s0 = arg1->player->m_pOptions;
    temp_s0->unk32 = func_0041648C((s32) arg1, (s32) temp_s0->unk32, 1, 0, 0xA, 0);
    return 0;
}
