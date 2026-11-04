#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern u8 D_8012F9D2;

void func_00407B80(CGameObjectInstance *obj, CAI *ai) {
    s32 var_a2;

    if (ai->m_AnimRepeatedCount != 0) {
        var_a2 = 0x13;
        if (D_8012F9D2 == 0) {
            var_a2 = 7;
        }
        func_00243414(obj, ai, var_a2);
    }
}
