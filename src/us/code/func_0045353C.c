/* Module-2 version of CheatsMenu_WarpSelect (the warp menu); here both exits also clear the multiplayer flag. */
#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"

/* Level select (warp) menu input: A warps to the selected level (or, on the
 * last entry, starts the credits/next mode), up and down move the selection
 * to the previous/next level whose key the player owns. */



#define PROGRESS (&gEngineApp_m_Game.m_Settings)
extern s32 sCheatsMenuSelection;
extern u8 gScene[];

extern s32 Controller_ConfirmPressed(s32 pad);
extern s32 Controller_MenuLeft(s32 pad);
extern s32 Controller_MenuRight(s32 pad);
extern void Menu_ClaimPressingController_2(Menu *c);
extern void CEngineApp__SetupFadeTo(void *game, s32 mode);
extern void CEngineApp__StartGameAtWarp(void *game, s32 level);
extern void CScene__RequestWarp_2(void *level, s32 id, s32 how);

static __inline__ u32 levelKey(s32 level) {
    u32 bit;

    switch (level) {
    case 0:
    default:
        bit = 0x10000;
        break;
    case 1:
        bit = 0x20000;
        break;
    case 2:
        bit = 0x40000;
        break;
    case 3:
        bit = 0x80000;
        break;
    case 4:
        bit = 0x100000;
        break;
    case 5:
        bit = 0x200000;
        break;
    case 6:
        bit = 0x800000;
        break;
    case 7:
        bit = 0x400000;
        break;
    case 8:
        bit = 0x1000000;
        break;
    case 9:
        bit = 0x2000000;
        break;
    case 10:
        bit = 0x4000000;
        break;
    }
    return bit;
}

/* The next level from the selection in direction step whose key is owned, or -1. */
static __inline__ s32 findLevel(s32 step) {
    s32 sel;
    s32 n;
    u32 keys;

    sel = sCheatsMenuSelection;
    keys = gEngineApp_m_Game.m_Settings.unlockedCheats;
    for (n = 0; n < 11; n++) {
        sel += step;
        if (sel > 10) {
            sel = 0;
        } else if (sel < 0) {
            sel = 10;
        }
        if (keys & levelKey(sel)) {
            return sel;
        }
    }
    return -1;
}

s32 CheatsMenu_WarpSelect_2(void *menu, Menu *c) {
    s32 id;

    switch (sCheatsMenuSelection) {
    case 0:
    default:
        id = 1000;
        break;
    case 1:
        id = 2000;
        break;
    case 2:
        id = 3000;
        break;
    case 3:
        id = 4000;
        break;
    case 4:
        id = 5000;
        break;
    case 5:
        id = 6000;
        break;
    case 6:
        id = 0x1CE9;
        break;
    case 7:
        id = 0x1D4D;
        break;
    case 8:
        id = 0x1DB1;
        break;
    case 9:
        id = 0x1DB2;
        break;
    case 10:
        id = 8000;
        break;
    }
    if (Controller_ConfirmPressed(c->controller) != 0) {
        if (id == 8000) {
            PROGRESS->multiplayer = 0;
            PROGRESS->playerCount = 1;
            Menu_ClaimPressingController_2(c);
            CEngineApp__SetupFadeTo(&gEngineApp, 16);
            return 1;
        }
        if (gEngineApp_m_Game.m_Settings.mode == 13) {
            CScene__RequestWarp_2(gScene, id, 2);
            return 0;
        }
        PROGRESS->multiplayer = 0;
        PROGRESS->playerCount = 1;
        Menu_ClaimPressingController_2(c);
        CEngineApp__StartGameAtWarp(&gEngineApp, id);
        return 1;
    }
    if (Controller_MenuLeft(c->controller) != 0) {
        sCheatsMenuSelection = findLevel(-1);
    } else if (Controller_MenuRight(c->controller) != 0) {
        sCheatsMenuSelection = findLevel(1);
    } else {
        return 0;
    }
    return 0;
}
