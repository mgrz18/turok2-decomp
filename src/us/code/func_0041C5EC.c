#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x2A];
    /* 0x2A */ u8 opt2A;
    /* 0x2B */ s8 opt2B;
} PlayerOptions;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} MenuItem;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);
extern PlayerOptions D_8011AB18;

s32 func_0041C5EC(s32 arg0, MenuItem *arg1) {
    PlayerOptions *temp_v0_2;
    PlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->opt2A = func_0041648C((s32) arg1, (s32) var_s0->opt2A, 8, 8, 0xF8, 0);
    return 0;
}
