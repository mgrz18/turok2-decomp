#include "common.h"
#include "structs/game_object.h"

s32 CCache__Request(s32, s32, s32, s32, s32, void *, void *, s32); /* extern */
void CCache__Unlock(s32, s32);                       /* extern */
void Model_TouchTextures(s32);                            /* extern */
extern u8 D_800A5CE0[];
extern void Model_DecompressCallbackExtra();

void CGameObjectInstance__TouchGeometry(CGameObjectInstance *arg0) {
    s32 temp_v0;

    temp_v0 = CCache__Request(0, (s32) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra,
                            D_800A5CE0, 0);
    if (temp_v0 != 0) {
        Model_TouchTextures(temp_v0);
        CCache__Unlock(0, temp_v0);
    }
}
