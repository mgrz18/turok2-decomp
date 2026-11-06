#include "common.h"
#include "structs/player.h"

/* The first player whose tag mode (options 0x3E) is 1. */
CPlayer *CPlayerPool__FindTagged(CPlayerList *list) {
    CPlayer *p;

    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if (p->m_pOptions->m_TagMode == 1) {
            return p;
        }
    }
    return NULL;
}
