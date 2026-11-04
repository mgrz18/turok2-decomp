#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"
#include "structs/boss.h"

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

void func_004457B4(void) {
    func_00243414(D_8012FB70.obj, &D_8012FB70.obj->m_AI, 0x14);
}
