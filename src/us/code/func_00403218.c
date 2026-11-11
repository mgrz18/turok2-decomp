#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void MantidQueen_UpdateMode13(sptr arg0, CAI *arg1) { /* the actor pointer-wide (sptr: s32 on the N64) */
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 8);
    }
}
