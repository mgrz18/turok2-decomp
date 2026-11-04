#include "common.h"
#include "structs/game_object.h"

void func_0021EBC0(CGameObjectInstance *arg0) {
    struct Mtx *temp_a1;

    temp_a1 = arg0->m_pmtDrawMtxs;
    arg0->m_pmtDrawMtxs = 0;
    arg0->m_pmfShadow = 0;
    arg0->m_dwFlags &= ~0x200;
    arg0->m_pmtLastFrameDrawMtxs = temp_a1;
}
