#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

/* Menu item handler: depending on the menu state, checks the item (PakMenu_CheckPak) and prints its text, or (states 1
 * and 2) resets the menu flags and opens the item's menu on its owner's camera. */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s8 id;
} ItemData;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *owner;
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
extern s32 PakMenu_CheckPak(Item *item, s32 id, s32 a2);
extern void *PakMenu_GetReturnMenu(Item *item);
extern void MenuPool_OpenMenu(void *dst, void *string, CPlayer *owner, ItemData *data, s32 a4);

static __inline__ s32 ItemId(Item *item) {
    s32 id;

    if (D_80130990 != 0) {
        id = D_800C2028;
    } else {
        id = item->data->id;
    }
    return id;
}

s32 PakConfirmMenu_SelectNo(s32 arg0, Item *item) {
    void *string;

    switch (D_801309B0) {
        case 4:
            if (PakMenu_CheckPak(item, ItemId(item), 1)) {
                D_801309BC = 1;
                MenuPool_OpenMenu(D_80119E2C, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(D_80119E2C, item->string, item->owner, item->data, 0);
            }
            break;
        case 3:
            if (PakMenu_CheckPak(item, ItemId(item), 1)) {
                D_801309BC = 1;
                MenuPool_OpenMenu(D_80119E2C, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(D_80119E2C, item->string, item->owner, item->data, 0);
            }
            break;
        case 0:
            if (PakMenu_CheckPak(item, ItemId(item), 1)) {
                D_801309BC = 1;
                MenuPool_OpenMenu(D_80119E2C, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(D_80119E2C, item->string, item->owner, item->data, 0);
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
            string = PakMenu_GetReturnMenu(item);
            if (string == NULL) {
                string = item->string;
            }
            if (item->string == D_0043BEF4 || item->string == D_0043BED0) {
                D_80130940 = 0;
            }
            MenuPool_OpenMenu(item->owner != NULL ? item->owner->m_pCamera->m_MenuPool : D_80119E2C, string, item->owner,
                          item->data, 0);
            break;
    }
    return 1;
}
