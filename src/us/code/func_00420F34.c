#include "common.h"

/* Menu item handler: depending on the menu state, checks the item (func_0041DD90) and prints its text, or (states 1
 * and 2) resets the menu flags and prints the item text at its owner's text position. */

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ u8 *text;
} Owner;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s8 id;
} ItemData;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Owner *owner;
    /* 0x20 */ ItemData *data;
    /* 0x24 */ void *string;
} Item;

extern s32 D_801309B0; /* menu state */
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_80130940;
extern s32 D_8011F148;
extern u8 D_80119E2C[];
extern u8 D_0043BED0[];
extern u8 D_0043BEF4[];
extern s32 func_0041DD90(Item *item, s32 id, s32 a2);
extern void *func_0041DC5C(Item *item);
extern void func_00416644(void *dst, void *string, Owner *owner, ItemData *data, s32 a4);

static __inline__ s32 ItemId(Item *item) {
    s32 id;

    if (D_80130990 != 0) {
        id = D_800C2028;
    } else {
        id = item->data->id;
    }
    return id;
}

s32 func_00420F34(s32 arg0, Item *item) {
    void *string;

    switch (D_801309B0) {
        case 4:
            if (func_0041DD90(item, ItemId(item), 1)) {
                D_801309BC = 1;
                func_00416644(D_80119E2C, item->string, item->owner, item->data, 0);
            } else {
                func_00416644(D_80119E2C, item->string, item->owner, item->data, 0);
            }
            break;
        case 3:
            if (func_0041DD90(item, ItemId(item), 1)) {
                D_801309BC = 1;
                func_00416644(D_80119E2C, item->string, item->owner, item->data, 0);
            } else {
                func_00416644(D_80119E2C, item->string, item->owner, item->data, 0);
            }
            break;
        case 0:
            if (func_0041DD90(item, ItemId(item), 1)) {
                D_801309BC = 1;
                func_00416644(D_80119E2C, item->string, item->owner, item->data, 0);
            } else {
                func_00416644(D_80119E2C, item->string, item->owner, item->data, 0);
            }
            break;
        case 1:
        case 2:
            D_801309BC = 1;
            D_80130940 = 0;
            if (D_80130990) {
                D_8011F148 = 1;
                return 1;
            }
            string = func_0041DC5C(item);
            if (string == NULL) {
                string = item->string;
            }
            if (item->string == D_0043BEF4 || item->string == D_0043BED0) {
                D_80130940 = 0;
            }
            func_00416644(item->owner != NULL ? item->owner->text + 0x53C : D_80119E2C, string, item->owner,
                          item->data, 0);
            break;
    }
    return 1;
}
