#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"


/* Whether some player sums, over the slots other than CPlayerPool__GetIndex's, at
 * least gEngineApp_m_Settings_match.scoreLimit in options field 0x10. */
s32 CPlayerPool__IsScoreLimitReached(CPlayerPool *list) {
    GameMatch *state;
    CPlayer *p;
    s32 skip;
    s32 sum;
    s32 i;

    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        state = &gEngineApp_m_Settings.match;
        skip = CPlayerPool__GetIndex(list, p);
        sum = 0;
        for (i = 0; i < 4; i++) {
            if (i != skip) {
                sum += ((CPlayerOptions *)p->m_pOptions)->m_Tagged[i];
            }
        }
        if (sum >= state->scoreLimit) {
            return 1;
        }
    }
    return 0;
}
