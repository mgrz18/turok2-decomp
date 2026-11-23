/* The port: the view put the active list head at N64 0x20 and the player's controller and link at
 * 0xA50/0x1320; the typed CPlayerPool/CPlayer (structs/player_list.h, structs/player.h) at their host
 * offsets. GameNoteMenu_SelectYes (loading a save) crashed on the NULL it returned. */
#include "common.h"
#include "structs/player.h"
void *CPlayerPool__FindByController(char *object, sptr arg1) {
    CPlayer *p;

    for (p = ((CPlayerPool *) object)->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if ((sptr) p->m_ContState.m_pContState == arg1) {
            return p;
        }
    }
    return 0;
}
