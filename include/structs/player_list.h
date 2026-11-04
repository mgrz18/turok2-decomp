#ifndef STRUCTS_PLAYER_LIST_H
#define STRUCTS_PLAYER_LIST_H

/* The player list (the start of CGame, D_80119870: structs/game.h), in its
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

/* All players (D_80119870, the start of the engine's globals; its active
 * list is D_80119890): the idle ones and the ones in the game. */
typedef struct {
    /* 0x00 */ struct CMemEntry *m_pmePlayers;
    /* 0x04 */ struct CPlayer *m_Players;
    /* 0x08 */ s32 m_nPlayers;
    /* 0x0C */ CPlayerCList m_IdleList;
    /* 0x20 */ CPlayerCList m_ActiveList;    /* D_80119890; m_nCount: the players in the game */
    /* 0x34 */ s32 m_bCinema;                    /* a cinema runs for the players */
    /* 0x38 */ s32 m_bCinemaStarted;
} CPlayerList; /* 0x3C known */

#endif
