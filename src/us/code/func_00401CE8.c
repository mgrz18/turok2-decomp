#include "common.h"
#include "structs/region.h"

void func_00401CE8(CGameRegion *arg0) {
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    if (arg0 != NULL) {
        temp_a0 = arg0->m_pNeighbours[0];
        arg0->m_wFlags &= 0x7FFF;
        if ((temp_a0 != NULL) && (temp_a0->m_wFlags & 0x8000)) {
            func_00401CE8(temp_a0);
        }
        temp_a0_2 = arg0->m_pNeighbours[1];
        if ((temp_a0_2 != NULL) && (temp_a0_2->m_wFlags & 0x8000)) {
            func_00401CE8(temp_a0_2);
        }
        temp_a0_3 = arg0->m_pNeighbours[2];
        if ((temp_a0_3 != NULL) && (temp_a0_3->m_wFlags & 0x8000)) {
            func_00401CE8(temp_a0_3);
        }
    }
}
