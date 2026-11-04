#include "common.h"
#include "structs/game_object.h"

s32 func_0020367C(s32, s32, s32, s32, s32, void *, void *, s32); /* resource load */
extern char D_800A5CE0[];
extern void func_0020B4D0(void);

void func_0021F358(CGameObjectInstance *arg0) {
    func_0020367C(0, (s32) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, func_0020B4D0, D_800A5CE0, 0);
}
