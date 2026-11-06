#include "common.h"
#include "rw/abu_protos.h"
#include "structs/world.h"

void func_0042C620(void *, int, int);

void CScene__WarpToLevel_2(CWorld *arg0, s32 arg1) {
    if (arg1 != arg0->m_nLevel) {
        arg0->m_bWarpSpawn = 0;
        func_0042C620(arg0, ~arg1, 0);
    }
}
