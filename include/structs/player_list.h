#ifndef STRUCTS_PLAYER_LIST_H
#define STRUCTS_PLAYER_LIST_H

/* The player list (the start of CGame, gEngineApp_m_Game: structs/game.h), in its
 * own header so that game.h does not need the whole player (structs/player.h
 * includes this). tools/check_structs.py checks the offsets. */

#ifndef RW_ABU_TYPES_H /* it has the base types too */
#include "common.h"
#endif

struct CPlayer;
struct CMemEntry;

/* Acclaim's intrusive list (CList in structs/camera.h) holding players. */
typedef struct {
    /* 0x00 */ struct CPlayer *m_pHead;
    /* 0x04 */ struct CPlayer *m_pTail;
    /* 0x08 */ s32 m_LastOffset;
    /* 0x0C */ s32 m_NextOffset;
    /* 0x10 */ s32 m_nCount;
} CPlayerCList; /* size 0x14 */

/* All players (gEngineApp_m_Game, the start of the engine's globals; its active
 * list is gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead): the idle ones and the ones in the game. Turok 3's
 * CPlayerPool (CPlayerPool__* methods); called CPlayerList here until round N3. */
typedef struct {
    /* 0x00 */ struct CMemEntry *m_pmePlayers;
    /* 0x04 */ struct CPlayer *m_Players;
    /* 0x08 */ s32 m_nPlayers;
    /* 0x0C */ CPlayerCList m_IdleList;
    /* 0x20 */ CPlayerCList m_ActiveList;    /* gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead; m_nCount: the players in the game */
    /* 0x34 */ s32 m_bCinema;                    /* a cinema runs for the players */
    /* 0x38 */ s32 m_bCinemaStarted;
} CPlayerPool; /* 0x3C known */

#endif
