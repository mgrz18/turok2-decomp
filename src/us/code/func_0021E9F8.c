#include "common.h"
#include "structs/game_object.h"

s32 func_0021E960();                            /* extern */

s32 func_0021E9F8(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    if ((arg1 < 0) || ((arg2 == 0) && (arg0->m_dwFlags & 0x400))) {
        return 0;
    }
    func_0021E960();
    return 1;
}
