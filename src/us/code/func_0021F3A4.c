#include "common.h"
#include "structs/game_object.h"

s32 func_0020367C(s32, s32, s32, s32, s32, void *, void *, s32); /* extern */
void func_002051F4(s32, s32);                       /* extern */
void func_0020BEE8(s32);                            /* extern */
extern u8 D_800A5CE0[];
extern void func_0020B4D0();

void func_0021F3A4(CGameObjectInstance *arg0) {
    s32 temp_v0;

    temp_v0 = func_0020367C(0, (s32) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, func_0020B4D0,
                            D_800A5CE0, 0);
    if (temp_v0 != 0) {
        func_0020BEE8(temp_v0);
        func_002051F4(0, temp_v0);
    }
}
