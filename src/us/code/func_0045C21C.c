/* Second-module copy of func_004232FC (first module; this one at link address 0x0042321C), calling this module's own functions at their link addresses. */
#include "common.h"

s32 func_00253640(s32, s32, s32);

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

s32 func_0045C21C(s32 arg0, MenuPage *arg1) {
    MenuData *temp_a0;
    MenuWidget *temp_a0_2;
    MenuWidget *temp_v0;

    temp_a0 = arg1->data;
    if (func_00253640((s32) temp_a0, 0, temp_a0->unk92C) != 0) {
        temp_v0 = arg1->widget;
        temp_v0->items[0].flags = temp_v0->items[0].flags | 0x01000000;
    } else {
        temp_a0_2 = arg1->widget;
        temp_a0_2->items[0].flags = temp_a0_2->items[0].flags & 0xFEFFFFFF;
    }
    return 0;
}
