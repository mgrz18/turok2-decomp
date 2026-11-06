#include "common.h"
#include "structs/game_object.h"

s32 CCache__Request(s32, s32, s32, s32, s32, void *, void *, s32); /* resource load */
extern char D_800A5CE0[];
extern void Model_DecompressCallbackExtra(void);

void CGameObjectInstance__LoadGeometry(CGameObjectInstance *arg0) {
    CCache__Request(0, (s32) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra, D_800A5CE0, 0);
}
