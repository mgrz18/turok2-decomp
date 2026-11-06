#ifndef GLOBALS_MENU_H
#define GLOBALS_MENU_H

/* The menus' state: the menu pool in the main camera and the menu screens' statics. */

#include "structs/menu.h"
#include "common.h"

/* The main camera's menu pool, gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_Menus
 * (CGame + 0x5BC): the menu code is passed its address. */
extern MenuPool gEngineApp_m_Game_m_MenuPool;

/* The menu screens' statics. */
extern char sMenuBindingText[0x40];      /* the controller-binding menu's label */
extern char sMenuBindingValue[0x40];     /* and its value */
extern s32 sMenuModelFade;
extern s32 gInSinglePlayerMenu;
extern s32 sCheatsMenuSelection;         /* CheatsMenu_Init: the first unlocked option */
extern s32 sEnterCheatResult;
extern s32 gCameraPoolHideViews;
extern s32 sMissionHub;
extern s32 sMissionFailed;
extern f32 gCreditsScroll;
extern f32 gCreditsTimeLeft;
extern s32 gCreditsDone;

/* Menu state of module 1's menu code (gMenuTextStyles, gCheatEntry,
 * gNameEntries are structs/menu.h's) and the HUD's loaded font. */

extern s32 gMenuCurrentFont;        /* the font/draw mode last selected (-1 resets) */
extern s32 gMpPlayerSpeed;          /* the multiplayer player-speed option, 0..15 */
extern s32 sSoundMenuSavedTrack;    /* Music_GetTrack() saved when the sound options open */
extern s32 sSoundMenuActive;        /* 1 while the sound options menu is open */
extern s32 sSoundMenuTrack;         /* the music track the sound menu plays */
extern u8 gCheatEntryText[0x64];    /* the typed cheat code */
extern s32 gLoadedFont;             /* which font texture COnScreen has loaded */
extern s32 sCreditsLine;            /* the first credits line on screen */

/* Menu statics: the final score screen timer, the resolution toggle delay, the paused rumble state */
extern f32 sFinalScoreTimer;
extern f32 sResolutionToggleDelay;
extern s32 sPauseRumbleOn[4];   /* the controllers' rumble state, saved while paused */

#endif
