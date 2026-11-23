#include "common.h"
#include "structs/game_object.h"

/* Byte arg2 of the AI variable buffer, read from its byte 0x24 on (m_VarBuffer[9]). */
s8 EnemyAI_GetBehaviorAt(s32 arg0, CAI *arg1, s32 arg2) {
#ifdef TARGET_PORT
    /* the port (round 9 x1): by member. The host CAI's variable buffer is not at N64 0x10C and
     * CEnemyAIVars holds a pointer (m_pGrabbed) before m_Behavior: N64 0x130 is m_Behavior[0]. */
    return ((s8 *) arg1->m_Enemy.m_Behavior)[arg2];
#else
    return *(s8 *) ((sptr) arg1 + arg2 + 0x130);
#endif
}
