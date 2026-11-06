#include "common.h"
#include "structs/game_object.h"

/* Calls the current AI mode's callback, if any. */
void Door_Step(s32 arg0, CAI *arg1) {
    s32 (*temp_v0_2)();
    CAIModeInfo *temp_v0;

    temp_v0 = arg1->m_pModeCurrent;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pfCallback;
        if (temp_v0_2 != NULL) {
            temp_v0_2();
        }
    }
}
