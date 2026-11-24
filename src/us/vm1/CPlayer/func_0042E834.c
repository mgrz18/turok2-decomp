/* Player code: CPlayerPool__Construct, 0x0042E834-0x0042E894 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "protos/list.h"

#define PLAYER_LAST T2_OFFSETOF(CPlayer, m_pLast)
#define PLAYER_NEXT T2_OFFSETOF(CPlayer, m_pNext)

void *memset(void *, s32, uptr);

void CPlayerPool__Construct(CPlayerPool *pl) {
    memset(pl, 0, sizeof(CPlayerPool));
    CList__Construct(&pl->m_IdleList, PLAYER_LAST, PLAYER_NEXT);
    CList__Construct(&pl->m_ActiveList, PLAYER_LAST, PLAYER_NEXT);
    pl->m_pmePlayers = 0;
    pl->m_Players = 0;
    pl->m_nPlayers = 0;
    pl->m_bCinema = 0;
    pl->m_bCinemaStarted = 0;
}
