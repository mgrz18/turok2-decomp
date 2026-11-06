#ifndef GLOBALS_MENU_H
#define GLOBALS_MENU_H

/* Menu state of module 1's menu code (gMenuTextStyles, gCheatEntry,
 * gNameEntries are structs/menu.h's) and the HUD's loaded font. */

#include "common.h"

extern s32 gMenuCurrentFont;        /* the font/draw mode last selected (-1 resets) */
extern s32 gMpPlayerSpeed;          /* the multiplayer player-speed option, 0..15 */
extern s32 sSoundMenuSavedTrack;    /* Music_GetTrack() saved when the sound options open */
extern s32 sSoundMenuActive;        /* 1 while the sound options menu is open */
extern s32 sSoundMenuTrack;         /* the music track the sound menu plays */
extern u8 gCheatEntryText[0x64];    /* the typed cheat code */
extern s32 gLoadedFont;             /* which font texture COnScreen has loaded */
extern s32 sCreditsLine;            /* the first credits line on screen */

#endif
