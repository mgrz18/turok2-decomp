#include "common.h"
#include "structs/game_object.h"

extern CListedInstance *D_8010FEB4;  /* active actors */

extern u8 *D_80119890;          /* current player */

void func_00408DCC(void) {
    CListedInstance *actor;

    *(f32 *) (D_80119890 + 0xAAC) = 450.0f;
    for (actor = D_8010FEB4; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            actor->m_Instance.m_AI.m_pPathTarget = NULL;
            actor->m_Instance.m_AI.m_pSightTarget = NULL;
            actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
}
