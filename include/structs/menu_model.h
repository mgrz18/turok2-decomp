#ifndef STRUCTS_MENU_MODEL_H
#define STRUCTS_MENU_MODEL_H

/* A menu's 3D model item (MenuItem type 3, structs/menu.h): its data says
 * which definition to show and where, and its 0x330-byte extra (allocated
 * after the window's items, Menu_Construct) holds a copy of that definition
 * and the instance drawn from it (MenuItem_DrawModel, MenuItem_SetModel). Kept apart
 * from menu.h, which does not need the instance's headers.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "structs/menu.h"
#include "structs/game_object.h"

/* A type-3 item's data (MenuItem part). */
typedef struct MenuModelPart {
    /* 0x00 */ s32 objType;         /* handed to CGameObjectInstance__Construct with the definition */
    /* 0x04 */ s32 intelType;       /* which definition: CScene__FindIntelligence(world, intelType, intelId) */
    /* 0x08 */ s32 intelId;
    /* 0x0C */ CVector3 pos;        /* in the window's camera view */
    /* 0x18 */ CVector3 scale;      /* in 284x222 screen units */
    /* 0x24 */ s32 width;           /* the model's box, in 284x222 screen units (MenuItem_Layout) */
    /* 0x28 */ s32 height;
    /* 0x2C */ f32 spin;            /* turned by spin * frame time each frame */
} MenuModelPart; /* size 0x30 */

/* A type-3 item's extra (MenuItem model). */
typedef struct MenuModel {
    /* 0x000 */ s32 intelType;      /* INTELLIGENCE_PLAYER (11): drawn with the player's colours */
    /* 0x004 */ CIntelligence intel; /* a copy of the definition found (its flags cleared) */
    /* 0x0A0 */ CGameObjectInstance instance;
    /* 0x328 */ s32 loaded;         /* set when made, cleared when no definition was found */
    /* 0x32C */ f32 height;         /* the definition's collision height (the model is lifted by it) */
} MenuModel; /* size 0x330 */

#endif
