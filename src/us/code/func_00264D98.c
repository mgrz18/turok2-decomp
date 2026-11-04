#include "common.h"
#include "structs/game_object.h"

void func_00264D98(s32 arg0, CAI *ai) {
    s32 (*temp_v0_2)();
    CAIModeInfo *temp_v0;

    temp_v0 = ai->m_pModeCurrent;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pfCallback;
        if (temp_v0_2 != NULL) {
            temp_v0_2();
        }
    }
}
