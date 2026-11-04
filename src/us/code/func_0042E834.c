/* Player code: func_0042E834, 0x0042E834-0x0042E894 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"

extern void func_00266C5C(void *p, s32 c, s32 n); /* memset */
extern void CList__Construct(void *list, s32 prevOffset, s32 nextOffset);

void func_0042E834(CPlayerList *pl) {
    func_00266C5C(pl, 0, sizeof(CPlayerList));
    CList__Construct(&pl->m_IdleList, 0x131C, 0x1320);
    CList__Construct(&pl->m_ActiveList, 0x131C, 0x1320);
    pl->m_pmePlayers = 0;
    pl->m_Players = 0;
    pl->m_nPlayers = 0;
    pl->m_bCinema = 0;
    pl->m_bCinemaStarted = 0;
}
