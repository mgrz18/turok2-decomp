#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

/* Menu item handler: depending on the menu state, checks the item (PakMenu_CheckPak) and prints its text, or (states 1
 * and 2) resets the menu flags and opens the item's menu on its owner's camera. */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s8 id;
} ItemData;

typedef struct {
#ifdef TARGET_PORT
    /* the host Menu's leading fields (structs/menu.h: cursor .. param), pointers 8 bytes wide */
    s16 cursor, offsetX, offsetY;
    u8 _pad6[2];
    void *block, *items;
    s32 count;
    void *desc, *param;
#else
    /* 0x00 */ u8 pad0[0x1C];
#endif
    /* 0x1C */ CPlayer *owner;
    /* 0x20 */ ItemData *data;
    /* 0x24 */ void *string;
} Item;

extern s32 PakMenu_CheckPak(Item *item, s32 id, s32 a2);
extern void *PakMenu_GetReturnMenu(Item *item);

static __inline__ s32 ItemId(Item *item) {
    s32 id;

    if (gPakMenuAnyPak != 0) {
        id = gPakMenuPort;
    } else {
        id = item->data->id;
    }
    return id;
}

s32 PakConfirmMenu_SelectNo(s32 arg0, Item *item) {
    void *string;

    switch (gPakConfirmKind) {
        case 4:
            if (PakMenu_CheckPak(item, ItemId(item), 1)) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            }
            break;
        case 3:
            if (PakMenu_CheckPak(item, ItemId(item), 1)) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            }
            break;
        case 0:
            if (PakMenu_CheckPak(item, ItemId(item), 1)) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, item->string, item->owner, item->data, 0);
            }
            break;
        case 1:
        case 2:
            gPakMenuLeaving = 1;
            gCameraPoolHideViews = 0;
            if (gPakMenuAnyPak) {
                gPakCheckDone = 1;
                return 1;
            }
            string = PakMenu_GetReturnMenu(item);
            if (string == NULL) {
                string = item->string;
            }
            if (item->string == &gSavePortalMenuDescB || item->string == &gSavePortalMenuDesc) {
                gCameraPoolHideViews = 0;
            }
            MenuPool_OpenMenu(item->owner != NULL ? item->owner->m_pCamera->m_MenuPool : &gEngineApp_m_Game_m_MenuPool, string, item->owner,
                          item->data, 0);
            break;
    }
    return 1;
}
