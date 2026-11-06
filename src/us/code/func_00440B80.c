#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

extern u8 D_8012F9D2;

void Primagen_UpdateMode14_2(s32 arg0, CAI *arg1) {
    s32 var_a2;

    if (arg1->m_AnimRepeatedCount != 0) {
        var_a2 = 0x13;
        if (D_8012F9D2 == 0) {
            var_a2 = 7;
        }
        CAI__SetMode(arg0, (s32) arg1, var_a2);
    }
}
