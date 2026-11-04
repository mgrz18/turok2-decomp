#include "common.h"
#include "structs/world.h"

s32 func_0020185C(s32 *, s32, s32, s32);

void func_00224F48(CWorld *arg0, s32 arg1, s32 *arg2, s32 arg3) {
    *arg2 = func_0020185C(arg0->m_pTextureSets, arg0->m_TextureSetsKey, arg1, arg3);
}
