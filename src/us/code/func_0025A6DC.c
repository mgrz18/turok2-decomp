#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AFF78;

void func_0025A6DC(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800AFF78, obj->header.m_pIntelligence->m_Enemy.m_wDartAnimFlags);
}
