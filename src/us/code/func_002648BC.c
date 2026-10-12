#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

M2C_UNK func_00220260(M2C_UNK *, void *, M2C_UNK);  /* extern */
extern M2C_UNK D_800F7078;

void func_002648BC(CGameObjectInstance *arg0) {
    func_00220260(&D_800F7078, arg0, 0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
}
