#include "common.h"
#include "structs/game_object.h"

/* The current AI mode (CAI.m_pModeCurrent): only its handler at 8 is used here. */
struct CAIModeInfo {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 (*m_pfHandler)();
};

void func_00265AF8(s32 arg0, CAI *arg1) {
    s32 (*temp_v0_2)();
    struct CAIModeInfo *temp_v0;

    temp_v0 = arg1->m_pModeCurrent;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pfHandler;
        if (temp_v0_2 != NULL) {
            temp_v0_2();
        }
    }
}
