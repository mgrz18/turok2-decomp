#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x2A];
    /* 0x2A */ u8 opt2A;
    /* 0x2B */ s8 opt2B;
} PlayerOptions;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Player *player;
} MenuItem;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);
extern PlayerOptions D_8011AB18;

s32 func_00454E68(s32 arg0, MenuItem *arg1) {
    PlayerOptions *temp_v0_2;
    PlayerOptions *var_s0;
    Player *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->options;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->opt2B = func_0041648C((s32) arg1, (s32) var_s0->opt2B, 1, 0, 1, 1);
    return 0;
}
