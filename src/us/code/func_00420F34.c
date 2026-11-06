#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/menu.h"

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

extern s32 gPakConfirmKind; /* menu state */
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuPort;
extern s32 gPakMenuLeaving;
extern s32 gCameraPoolHideViews;
extern s32 gPakCheckDone;
extern u8 gSavePortalMenuDesc[];
extern u8 gSavePortalMenuDescB[];
extern s32 PakMenu_CheckPak(Item *item, s32 id, s32 a2);
extern void *PakMenu_GetReturnMenu(Item *item);
extern void MenuPool_OpenMenu(void *dst, void *string, CPlayer *owner, ItemData *data, s32 a4);

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
            if (item->string == gSavePortalMenuDescB || item->string == gSavePortalMenuDesc) {
                gCameraPoolHideViews = 0;
            }
            MenuPool_OpenMenu(item->owner != NULL ? item->owner->m_pCamera->m_MenuPool : &gEngineApp_m_Game_m_MenuPool, string, item->owner,
                          item->data, 0);
            break;
    }
    return 1;
}
