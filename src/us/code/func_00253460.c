#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

/* Once every player has options byte 0x3D set to 1, turns the pending flag
 * into the done flag and returns 1. */
s32 CPlayerPool__CheckAllReady(CPlayerPool *list) {
    GameMatch *state = &gEngineApp_m_Settings_match;
    CPlayer *p;
    s32 n;

    if (state->done) {
        return 0;
    }
    if (state->over) {
        n = 0;
        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            if (((CPlayerOptions *)p->m_pOptions)->unk_0x3D == 1) {
                n++;
            }
        }
        if (n == list->m_ActiveList.m_nCount) {
            GameMatch *s = &gEngineApp_m_Settings_match;

            s->done = 1;
            s->over = 0;
            return 1;
        }
    }
    return 0;
}
