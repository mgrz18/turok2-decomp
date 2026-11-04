#include "common.h"

/* The game options at D_8011AAD8 (the game state at D_80119870 + 0x1268):
 * the bytes the option menus step. */
typedef struct {
    /* 0x00 */ u8 pad0[0x21];
    /* 0x21 */ s8 unk21;            /* 1..99 */
    /* 0x22 */ s8 unk22;            /* 1..13 */
    /* 0x23 */ u8 pad23[0x28 - 0x23];
    /* 0x28 */ s8 unk28;            /* 1..8 */
    /* 0x29 */ u8 unk29;            /* 1..1 */
    /* 0x2A */ u8 pad2A[0x2E - 0x2A];
    /* 0x2E */ u8 unk2E;            /* 1..1 */
} GameOptions;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);
extern GameOptions D_8011AAD8;

s32 func_00451B4C(s32 arg0, s32 arg1) {
    GameOptions *var_s0;

    var_s0 = &D_8011AAD8;
    var_s0 = &D_8011AAD8;
    D_8011AAD8.unk22 = func_0041648C(arg1, (s32) D_8011AAD8.unk22, 1, 0, 0xD, 1);
    return 0;
}
