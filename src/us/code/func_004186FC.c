#include "common.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

/* Option values at D_8011AAD8 adjusted by the menu (func_0041648C steps a
 * value within [min, max]). */
typedef struct {
    /* 0x00 */ u8 pad0[0x25];
    /* 0x25 */ s8 opt25;
    /* 0x26 */ u8 pad26[0x28 - 0x26];
    /* 0x28 */ s8 opt28;
    /* 0x29 */ u8 pad29[0x2B - 0x29];
    /* 0x2B */ s8 opt2B;
    /* 0x2C */ u8 opt2C;
    /* 0x2D */ s8 opt2D;
    /* 0x2E */ u8 opt2E;
    /* 0x2F */ u8 pad2F;
    /* 0x30 */ s8 opt30;
} MenuOptions;
extern MenuOptions D_8011AAD8;

s32 func_004186FC(s32 arg0, s32 arg1) {
    MenuOptions *var_s0;

    var_s0 = &D_8011AAD8;
    var_s0 = &D_8011AAD8;
    D_8011AAD8.opt28 = func_0041648C(arg1, (s32) D_8011AAD8.opt28, 1, 0, 8, 1);
    return 0;
}
