#include "common.h"
#include "structs/game_object.h"

extern CListedInstance *D_8010FEB4;  /* active actors */

/* Looks for a live player actor; the result is not used. */
void func_00408FB0(void) {
    CListedInstance *actor;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            break;
        }
    }
}
