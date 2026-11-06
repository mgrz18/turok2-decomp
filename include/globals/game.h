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
extern u32 gEngineApp_m_Settings_cheatFlags;     /* .cheatFlags (offset 0, so the address of gEngineApp_m_Settings: symbols_manual) */
extern u8 gEngineApp_m_Settings_multiplayer;      /* .multiplayer */
extern u8 gEngineApp_m_Settings_unk180;           /* .unk180 */
extern s32 gEngineApp_m_Settings_mode;            /* .mode */
extern s32 gEngineApp_m_Settings_nextMode;        /* .nextMode */
extern GameMatch gEngineApp_m_Settings_match;     /* .match */
extern s32 gEngineApp_m_Settings_match_teams;     /* .match.teams */
extern s32 gEngineApp_m_Settings_match_tag;       /* .match.tag */
extern s32 gEngineApp_m_Settings_match_unk44;     /* .match.unk44 */

/* Members of gEngineApp_m_Game with symbols of their own (kept where the ROM
 * bases the access on them). */
extern struct CPlayer *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead; /* .m_PlayerList.m_ActiveList.m_pHead */
extern struct CPlayer *gEngineApp_m_Game_m_PlayerList_m_Players;            /* .m_PlayerList.m_Players */
extern s32 gEngineApp_m_Game_m_PlayerList_m_nPlayers;                       /* .m_PlayerList.m_nPlayers */
extern CAutomap gEngineApp_m_Game_m_DebugLines;                             /* .m_Automap */

#endif
