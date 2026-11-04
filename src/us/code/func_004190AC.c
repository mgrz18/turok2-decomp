#include "common.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

/* Game options block (D_8011AAD8); the menu handlers below step one setting each. */
typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x20 - 0x4];
    /* 0x20 */ s8 opt20;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ u8 opt24;
    /* 0x25 */ u8 pad25[2];
    /* 0x27 */ s8 opt27;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ s8 opt2C;
} GameOptions;

extern GameOptions D_8011AAD8;

s32 func_004190AC(s32 arg0, s32 arg1) {
    GameOptions *var_s0;

    var_s0 = &D_8011AAD8;
    var_s0 = &D_8011AAD8;
    D_8011AAD8.opt2C = func_0041648C(arg1, (s32) D_8011AAD8.opt2C, 1, 0, 0xD, 1);
    return 0;
}
