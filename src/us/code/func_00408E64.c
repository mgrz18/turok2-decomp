#include "common.h"
#include "structs/game_object.h"

extern CListedInstance *D_8010FEB4;

/* Is any actor whose definition kind is 1 still alive? */
s32 func_00408E64(void) {
    CListedInstance *actor;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}
