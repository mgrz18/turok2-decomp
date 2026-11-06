#ifndef GLOBALS_MENU_H
#define GLOBALS_MENU_H

/* The menus' state: the menu pool in the main camera and the menu screens' statics. */

#include "structs/menu.h"

/* The main camera's menu pool, gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_Menus
 * (CGame + 0x5BC): the menu code is passed its address. */
extern MenuPool gEngineApp_m_Game_m_MenuPool;

#endif
