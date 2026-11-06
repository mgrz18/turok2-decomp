#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"

extern s32 CPlayerPool__GetIndex(CPlayerPool *list, CPlayer *player);

/* Whether the frag limit is reached: by either team's total in team games,
 * otherwise by one player's frags (the slot CPlayerPool__GetIndex names left out). */
s32 CPlayerPool__IsFragLimitReached(CPlayerPool *list) {
    GameMatch *state = &gEngineApp_m_Settings.match;
    CPlayer *p;
    s32 i;

    if (state->teams) {
        s32 team1 = 0;
        s32 team0 = 0;
        s32 skip;

        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            skip = CPlayerPool__GetIndex(list, p);
            for (i = 0; i < 4; i++) {
                if (i != skip) {
                    switch (p->m_nDeathmatchTeamSelected) {
                        case 0:
                            team0 += p->m_pOptions->m_Killed[i];
                            break;
                        case 1:
                            team1 += p->m_pOptions->m_Killed[i];
                            break;
                    }
                }
            }
            if (team0 >= gEngineApp_m_Settings.match.scoreLimit) {
                return 1;
            }
            if (team1 >= gEngineApp_m_Settings.match.scoreLimit) {
                return 1;
            }
        }
    } else {
        GameMatch *gs = state;

        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            s32 skip;
            s32 sum;

            skip = CPlayerPool__GetIndex(list, p);
            sum = 0;
            for (i = 0; i < 4; i++) {
                if (i != skip) {
                    sum += p->m_pOptions->m_Killed[i];
                }
            }
            if (sum >= gs->scoreLimit) {
                return 1;
            }
        }
    }
    return 0;
}
