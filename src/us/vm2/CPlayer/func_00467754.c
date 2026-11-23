/* Second VM module copy of CPlayerPool__Construct (same source, linked 0x0042E754). */
#include "common.h"
#include "structs/player.h"
#include "protos/list.h"

extern void memset(void *p, s32 c, sptr n); /* memset */

void CPlayerPool__Construct_2(CPlayerPool *pl) {
    memset(pl, 0, sizeof(CPlayerPool));
    CList__Construct(&pl->m_IdleList, T2_OFFSETOF(CPlayer, m_pLast), T2_OFFSETOF(CPlayer, m_pNext));
    CList__Construct(&pl->m_ActiveList, T2_OFFSETOF(CPlayer, m_pLast), T2_OFFSETOF(CPlayer, m_pNext));
    pl->m_pmePlayers = 0;
    pl->m_Players = 0;
    pl->m_nPlayers = 0;
    pl->m_bCinema = 0;
    pl->m_bCinemaStarted = 0;
}
