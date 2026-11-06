#ifndef GLOBALS_MENU_H
#define GLOBALS_MENU_H

/* The menus' state: the menu pool in the main camera and the menu screens' statics. */

#include "structs/menu.h"

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

#endif
