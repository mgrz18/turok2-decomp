#include "common.h"

/* The AI fields this mode callback touches; 0x10C-0x13B is CAI::m_VarBuffer,
 * the per-mode scratch. */
typedef struct {
    /* 0x000 */ u8 pad0[0xC7];
    /* 0x0C7 */ s8 m_AnimRepeatedCount;
    /* 0x0C8 */ u8 padC8[0x133 - 0xC8];
    /* 0x133 */ s8 modeIndex;
} AI;

s32 func_00243414(s32, s32, s32);

void func_0025F6DC(s32 arg0, AI *arg1) {
    arg1->modeIndex = 2;
    if (arg1->m_AnimRepeatedCount != 0) {
        func_00243414(arg0, (s32) arg1, 4);
    }
}
