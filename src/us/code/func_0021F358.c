/* One source file: CGameObjectInstance__LoadGeometry and
 * CGameObjectInstance__TouchGeometry name the same "anim geometry" literal (GCC
 * shares a string only within a file), the file's whole .rodata. */
#include "common.h"
#include "structs/game_object.h"
#include "protos/cache.h"

void Model_TouchTextures(sptr);
extern void Model_DecompressCallbackExtra(void);

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
void * CGameObjectInstance__LoadGeometry(CGameObjectInstance *arg0) {
    return CCache__Request(0, (sptr) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra, "anim geometry", 0);
}
#else
void CGameObjectInstance__LoadGeometry(CGameObjectInstance *arg0) {
    CCache__Request(0, (sptr) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra, "anim geometry", 0);
}
#endif

void CGameObjectInstance__TouchGeometry(CGameObjectInstance *arg0) {
    sptr temp_v0;

    temp_v0 = CCache__Request(0, (sptr) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra,
                            "anim geometry", 0);
    if (temp_v0 != 0) {
        Model_TouchTextures(temp_v0);
        CCache__Unlock(0, temp_v0);
    }
}
