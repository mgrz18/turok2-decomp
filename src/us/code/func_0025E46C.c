#include "common.h"
#include "structs/game_object.h"

/* Byte arg2 of the AI variable buffer, read from its byte 0x24 on (m_VarBuffer[9]). */
s8 EnemyAI_GetBehaviorAt(s32 arg0, CAI *arg1, s32 arg2) {
    return *(s8 *) ((s32) arg1 + arg2 + 0x130);
}
