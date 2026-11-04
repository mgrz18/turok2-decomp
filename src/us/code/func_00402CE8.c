#include "common.h"
#include "structs/game_object.h"

s32 func_002666B0(s32, s32, s32);           /* extern */
extern s32 D_8012F5C0;

void func_00402CE8(CGameObjectInstance *arg0) {
    func_002666B0(D_8012F5C0, D_8012F5C0 + 0x140, 0x5AAA);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
