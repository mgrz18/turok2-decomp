#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

extern s8 D_8012F9D0;

void Primagen_UpdateMode16(s32 arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        D_8012F9D0 = 0;
        CAI__SetMode(arg0, (s32) ai, 2);
    }
}
