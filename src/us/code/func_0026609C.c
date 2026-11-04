#include "common.h"
#include "structs/game_object.h"

struct CAIModeInfo {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 (*func)();
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 unk10;
};

void func_0026609C(s32 arg0, CAI *arg1) {
    s32 (*temp_v0_2)();
    struct CAIModeInfo *temp_v0;

    temp_v0 = arg1->m_pModeCurrent;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->func;
        if (temp_v0_2 != NULL) {
            temp_v0_2();
        }
    }
}
