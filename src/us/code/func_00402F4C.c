#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

M2C_UNK func_002666B0(s32, s32, M2C_UNK);           /* extern */
extern s32 D_8012F5C0;

void func_00402F4C(CGameObjectInstance *arg0) {
    func_002666B0(D_8012F5C0, D_8012F5C0 + 0x140, 0x5AB4);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
}
