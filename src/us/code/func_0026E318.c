#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

M2C_UNK func_0021A7EC();                            /* extern */

void func_0026E318(CGameObjectInstance *arg0) {
    func_0021A7EC();
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}
