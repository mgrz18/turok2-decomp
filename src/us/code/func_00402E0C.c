#include "common.h"
#include "structs/game_object.h"

s32 func_002666B0(s32, s32, s32);           /* extern */
extern s32 D_8012F5C0;

void func_00402E0C(CGameObjectInstance *arg0) {
    s32 *flags = &arg0->m_dwFlags;

    *flags = (s32) (*flags & ~0x2000);
    func_002666B0(D_8012F5C0, D_8012F5C0 + 0x140, 0x5ABE);    /* obj, &obj->m_AI */
}
