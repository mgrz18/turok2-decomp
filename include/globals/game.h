#ifndef GLOBALS_GAME_H
#define GLOBALS_GAME_H

/* The game state inside the engine (gEngineApp.m_Game, CGame) and its settings
 * (gEngineApp.m_Settings, GameSettings): structs/game.h. The symbols below are
 * the same memory as the gEngineApp members; files reach them by symbol where
 * the ROM bases its loads there. */

#include "structs/game.h"

extern CGame gEngineApp_m_Game;              /* gEngineApp.m_Game */
extern GameSettings gEngineApp_m_Settings;   /* gEngineApp.m_Settings, gEngineApp_m_Game.m_Settings */

#endif
