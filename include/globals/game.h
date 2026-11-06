#ifndef GLOBALS_GAME_H
#define GLOBALS_GAME_H

/* The game state inside the engine (gEngineApp.m_Game, CGame) and its settings
 * (gEngineApp.m_Settings, GameSettings): structs/game.h. The symbols below are
 * the same memory as the gEngineApp members; files reach them by symbol where
 * the ROM bases its loads there. */

#include "structs/game.h"

extern CGame gEngineApp_m_Game;              /* gEngineApp.m_Game */
extern GameSettings gEngineApp_m_Settings;   /* gEngineApp.m_Settings, gEngineApp_m_Game.m_Settings */

/* Fields of gEngineApp_m_Settings with symbols of their own: where the ROM
 * bases a load on the field (GCC would base it on the struct otherwise). */
extern u32 gEngineApp_m_Settings_unlockedCheats;  /* .unlockedCheats */
extern s32 gEngineApp_m_Settings_difficulty;      /* .difficulty */
extern u8 gEngineApp_m_Settings_unk14;            /* .unk14 */
extern u8 gEngineApp_m_Settings_unk15;            /* .unk15 */
extern u8 gEngineApp_m_Settings_unk16;            /* .unk16 */
extern u8 gEngineApp_m_Settings_unk17;            /* .unk17 */
extern u8 gEngineApp_m_Settings_unk18;            /* .unk18 */
extern u8 gEngineApp_m_Settings_multiplayer;      /* .multiplayer */
extern u8 gEngineApp_m_Settings_unk1B;            /* .unk1B */
extern u8 gEngineApp_m_Settings_unk180;           /* .unk180 */
extern s32 gEngineApp_m_Settings_mode;            /* .mode */
extern s32 gEngineApp_m_Settings_nextMode;        /* .nextMode */
extern s32 gEngineApp_m_Settings_unk1C8;          /* .unk1C8 */
extern s32 gEngineApp_m_Settings_paused;          /* .paused */

#endif
