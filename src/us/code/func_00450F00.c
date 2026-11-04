#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

/* Multiplayer options kept per player (CPlayer.m_pOptions; see
 * func_00247C10.c): only the byte at 0x32 here. */
struct CPlayerOptions {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u8 unk32;            /* 1..10 */
};

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00450F00(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_s0;

    temp_s0 = ((CPlayer *) arg1->player)->m_pOptions;
    temp_s0->unk32 = func_0041648C((s32) arg1, (s32) temp_s0->unk32, 1, 0, 0xA, 0);
    return 0;
}
