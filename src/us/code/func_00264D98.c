#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 (*func)();
} CAIModeInfo;

void func_00264D98(s32 arg0, CAI *ai) {
    s32 (*temp_v0_2)();
    CAIModeInfo *temp_v0;

    temp_v0 = (CAIModeInfo *) ai->m_pModeCurrent;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->func;
        if (temp_v0_2 != NULL) {
            temp_v0_2();
        }
    }
}
