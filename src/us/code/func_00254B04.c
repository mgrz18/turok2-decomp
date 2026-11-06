#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

/* The index-th (0-3) player with a camera whose camera word 0x54C and
 * options byte 0x3F are both clear. */
CPlayer *CPlayerPool__GetViewPlayer(CPlayerPool *list, u32 index) {
    CPlayer *p = NULL;
    s32 n = 0;

    if (index < 4) {
        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            if (p->m_pCamera != NULL && *(s32 *)((u8 *)p->m_pCamera + 0x54C) == 0
                && ((CPlayerOptions *)p->m_pOptions)->m_LastTagMode == 0) {
                if (n == index) {
                    return p;
                }
                n++;
            }
        }
    }
    return p;
}
