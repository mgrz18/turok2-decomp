#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s8 D_8012F9D0;

void func_00440BE8(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        D_8012F9D0 = 0;
        func_00243414(obj, ai, 2);
    }
}
