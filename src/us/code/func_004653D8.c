#include "common.h"
#include "rw/abu_protos.h"
#include "structs/world.h"

void CScene__RequestWarp_2(void *, int, int);

void CScene__WarpToLevel_2(CScene *arg0, s32 arg1) {
    if (arg1 != arg0->m_nLevel) {
        arg0->m_bWarpSpawn = 0;
        CScene__RequestWarp_2(arg0, ~arg1, 0);
    }
}
