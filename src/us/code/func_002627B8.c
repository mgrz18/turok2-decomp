#include "common.h"
#include "structs/game_object.h"

/* What this calls of the current mode entry (CAI::m_pModeCurrent). */
typedef struct {
    /* 0x0 */ u8 pad0[8];
    /* 0x8 */ s32 (*onEvent)();
} ModeInfo;

void func_002627B8(s32 arg0, CAI *arg1) {
    s32 (*temp_v0_2)();
    ModeInfo *temp_v0;

    temp_v0 = (ModeInfo *) arg1->m_pModeCurrent;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->onEvent;
        if (temp_v0_2 != NULL) {
            temp_v0_2();
        }
    }
}
