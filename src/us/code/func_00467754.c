/* Second VM module copy of CPlayerPool__Construct (same source, linked 0x0042E754). */
#include "common.h"
#include "structs/player.h"

extern void memset(void *p, s32 c, s32 n); /* memset */
extern void CList__Construct(void *list, s32 prevOffset, s32 nextOffset);

void CPlayerPool__Construct_2(CPlayerPool *pl) {
    memset(pl, 0, sizeof(CPlayerPool));
    CList__Construct(&pl->m_IdleList, 0x131C, 0x1320);
    CList__Construct(&pl->m_ActiveList, 0x131C, 0x1320);
    pl->m_pmePlayers = 0;
    pl->m_Players = 0;
    pl->m_nPlayers = 0;
    pl->m_bCinema = 0;
    pl->m_bCinemaStarted = 0;
}
