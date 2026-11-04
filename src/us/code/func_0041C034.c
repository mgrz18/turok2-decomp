#include "common.h"
#include "structs/menu.h"

extern s32 D_800B3F00;
extern s32 D_800B3F18;
/* Option values adjusted by the menu (func_0041648C steps a value within
 * [min, max]); the same block as D_8011AAD8. */
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

typedef struct {
    /* 0x000 */ u8 pad0[0x514];
    /* 0x514 */ s32 unk514;
    /* 0x518 */ MenuOptions *options;
    /* 0x51C */ s32 unk51C;
    /* 0x520 */ u8 pad520[0x92C - 0x520];
    /* 0x92C */ s32 unk92C;
    /* 0x930 */ u8 pad930[0xA50 - 0x930];
    /* 0xA50 */ s32 unkA50;
    /* 0xA54 */ u8 padA54[0xBF4 - 0xA54];
    /* 0xBF4 */ s32 unkBF4;
    /* 0xBF8 */ s32 unkBF8;
} MenuData;

extern MenuOptions D_8011AB18;

s32 func_0041C034(MenuItem *arg0, Menu *arg1) {
    MenuOptions *temp_v0_2;
    MenuOptions *var_v1;
    u8 temp_v1;
    MenuData *temp_v0;

    temp_v0 = arg1->player;
    var_v1 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->options;
        if (temp_v0_2 != NULL) {
            var_v1 = temp_v0_2;
        }
    }
    temp_v1 = var_v1->opt2C;
    if (temp_v1 != 0) {
        if (temp_v1 == 1) {
            arg0->table = &D_800B3F00;
        }
    } else {
        arg0->table = &D_800B3F18;
    }
    return 0;
}
