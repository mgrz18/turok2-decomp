#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

extern s32 CPlayerPool__GetIndex(CPlayerList *list, CPlayer *player);

/* Whether some player sums, over the slots other than CPlayerPool__GetIndex's, at
 * least D_8011ACB0.scoreLimit in options field 0x10. */
s32 CPlayerPool__IsScoreLimitReached(CPlayerList *list) {
    GameMatch *state;
    CPlayer *p;
    s32 skip;
    s32 sum;
    s32 i;

    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        state = &D_8011ACB0;
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
