#include "common.h"
#include "rw/abu_protos.h"

extern s32 D_8011ACE4;

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


/* Byte-offset field access: member references here would let the
 * scheduler move the D_8011ACE4 load and change the register choice. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void func_0041CDBC(MenuPage *arg0) {
    MenuWidget *temp_a0;
    MenuWidget *temp_v0;
    MenuData *temp_v1;
    MenuWidget *temp_v1_2;
    MenuWidget *temp_v1_3;
    MenuWidget *temp_v1_4;
    MenuWidget *temp_v1_5;
    MenuWidget *temp_v1_6;

    temp_v1 = arg0->data;
    if (temp_v1 != NULL) {
        temp_v1->unkBF8 = 1;
    }
    temp_v1_2 = arg0->widget;
    FIELD_AT(temp_v1_2, s32, MenuWidget, items[9].flags) = (s32) (FIELD_AT(temp_v1_2, s32, MenuWidget, items[9].flags) | 0x01800000);
    temp_v1_3 = arg0->widget;
    FIELD_AT(temp_v1_3, s32, MenuWidget, items[10].flags) = (s32) (FIELD_AT(temp_v1_3, s32, MenuWidget, items[10].flags) | 0x01800000);
    temp_v1_4 = arg0->widget;
    FIELD_AT(temp_v1_4, s32, MenuWidget, items[11].flags) = (s32) (FIELD_AT(temp_v1_4, s32, MenuWidget, items[11].flags) | 0x01800000);
    temp_v1_5 = arg0->widget;
    FIELD_AT(temp_v1_5, s32, MenuWidget, items[12].flags) = (s32) (FIELD_AT(temp_v1_5, s32, MenuWidget, items[12].flags) & 0xFE7FFFFF);
    temp_v1_6 = arg0->widget;
    FIELD_AT(temp_v1_6, s32, MenuWidget, items[13].flags) = (s32) (FIELD_AT(temp_v1_6, s32, MenuWidget, items[13].flags) & 0xFE7FFFFF);
    if (D_8011ACE4 != 0) {
        temp_v0 = arg0->widget;
        FIELD_AT(temp_v0, s32, MenuWidget, items[5].flags) = (s32) (FIELD_AT(temp_v0, s32, MenuWidget, items[5].flags) | 0x01000000);
        return;
    }
    temp_a0 = arg0->widget;
    FIELD_AT(temp_a0, s32, MenuWidget, items[5].flags) = (s32) (FIELD_AT(temp_a0, s32, MenuWidget, items[5].flags) & 0xFEFFFFFF);
}
