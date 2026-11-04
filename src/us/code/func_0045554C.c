#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

/* The player options (CPlayer.m_pOptions; D_8011AB18 holds the defaults). */
struct CPlayerOptions {
    /* 0x00 */ u8 pad0[0x2A];
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 pad2B[2];
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ s8 unk30;
    /* 0x31 */ u8 pad31[0x3D - 0x31];
    /* 0x3D */ s8 unk3D;
};

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

extern struct CPlayerOptions D_8011AB18;

s32 func_0045554C(s32 arg0, Menu *arg1) {
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
    var_s0->unk2A = func_0041648C((s32) arg1, var_s0->unk2A, 8, 8, 0xF8, 0);
    return 0;
}
