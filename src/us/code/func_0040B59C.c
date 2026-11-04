#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

/* What this reads of the current mode entry (CAI::m_pModeCurrent). */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s8 state;
} ModeAnim;

void func_0040B59C(s32 arg0, CAI *arg1) {
    ((ModeAnim *) arg1->m_pModeCurrent->m_pData)->state = 7;
    if (arg1->m_AnimRepeatedCount != 0) {
        func_00243414(arg0, (s32) arg1, 0xB);
    }
}
