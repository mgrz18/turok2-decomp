#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

void func_004472B4(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        func_00243414(obj, ai, 2);
    }
}
