#include "common.h"

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

/* A menu item: its widget at 0xC (flag 0x01000000 in widget->flags hides
 * it) and the player data at 0x1C. */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 flags;
} MenuItem; /* size 0x28 */

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ MenuItem items[14];
} MenuWidget;

typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ MenuWidget *widget;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ MenuData *data;
} MenuPage;
extern MenuOptions D_8011AB18;

/* The player's options, or the defaults at D_8011AB18. */
MenuOptions *func_00454C9C(MenuPage *arg0) {
    MenuOptions *temp_v1_2;
    MenuOptions *var_v0;
    MenuData *temp_v1;

    temp_v1 = arg0->data;
    var_v0 = &D_8011AB18;
    if (temp_v1 != NULL) {
        temp_v1_2 = temp_v1->options;
        if (temp_v1_2 != NULL) {
            var_v0 = temp_v1_2;
        }
    }
    return var_v0;
}
