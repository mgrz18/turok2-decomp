#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

/* Multiplayer options kept per player (CPlayer.m_pOptions; see
 * func_00247C10.c); D_8011AB18 holds the defaults. */
struct CPlayerOptions {
    /* 0x00 */ u8 pad0[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ u8 unk2C;
};

s32 func_0041648C(s32, s32, s32, s32, s32, s32);
extern struct CPlayerOptions D_8011AB18;

s32 func_00455420(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_v0_2;
    struct CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->unk29 = func_0041648C((s32) arg1, (s32) var_s0->unk29, 8, 8, 0xF8, 0);
    return 0;
}
