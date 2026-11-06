#include "common.h"
#include "structs/world.h"

s32 CScene__RequestWarp(s32, s32, s32);

void CScene__WarpToLevel(CScene *arg0, s32 arg1) {
    if (arg1 != arg0->m_nLevel) {
        arg0->m_bWarpSpawn = 0;
        CScene__RequestWarp((s32) arg0, ~arg1, 0);
    }
}
