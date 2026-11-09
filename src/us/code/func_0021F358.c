/* One source file: CGameObjectInstance__LoadGeometry and
 * CGameObjectInstance__TouchGeometry name the same "anim geometry" literal (GCC
 * shares a string only within a file), the file's whole .rodata. */
#include "common.h"
#include "structs/game_object.h"
#include "protos/cache.h"

void Model_TouchTextures(s32);
extern void Model_DecompressCallbackExtra(void);

void CGameObjectInstance__LoadGeometry(CGameObjectInstance *arg0) {
    CCache__Request(0, (s32) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra, "anim geometry", 0);
}

void CGameObjectInstance__TouchGeometry(CGameObjectInstance *arg0) {
    s32 temp_v0;

    temp_v0 = CCache__Request(0, (s32) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra,
                            "anim geometry", 0);
    if (temp_v0 != 0) {
        Model_TouchTextures(temp_v0);
        CCache__Unlock(0, temp_v0);
    }
}
