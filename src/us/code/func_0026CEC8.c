#include "common.h"
#include "structs/region.h"

/* Clears flag 1 over the subtree. */
void func_0026CEC8(CGameRegion *arg0) {
    u16 temp_v1;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    temp_v1 = arg0->m_wFlags;
    if (temp_v1 & 1) {
        temp_a0 = arg0->m_pNeighbours[0];
        arg0->m_wFlags = (u16) (temp_v1 & 0xFFFE);
        if (temp_a0 != NULL) {
            func_0026CEC8(temp_a0);
        }
        temp_a0_2 = arg0->m_pNeighbours[1];
        if (temp_a0_2 != NULL) {
            func_0026CEC8(temp_a0_2);
        }
        temp_a0_3 = arg0->m_pNeighbours[2];
        if (temp_a0_3 != NULL) {
            func_0026CEC8(temp_a0_3);
        }
    }
}
