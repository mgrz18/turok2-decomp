#include "common.h"
#include "structs/game_object.h"

/* Sets the condition timer to arg2 (0 always; otherwise only to raise it),
 * restarting the mode time when the timer was 0. */
void AI_RaiseConditionTimer(s32 arg0, CAI *arg1, f32 arg2) {
    if ((arg2 == 0.0f) || (arg1->m_ConditionTimer < arg2)) {
        if (arg1->m_ConditionTimer == 0.0f) {
            arg1->m_ModeTime = 0.0f;
        }
        arg1->m_ConditionTimer = arg2;
    }
}
