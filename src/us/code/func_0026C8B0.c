#include "common.h"
#include "structs/region.h"

/* Marks arg0 and, recursively, every node linked to it with id arg1. */
void CGameRegion__FloodSetFlag4Same(CGameRegion *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    temp_s0 = arg0->m_nInfo;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->m_wFlags;
        if (!(temp_v1 & 4)) {
            temp_a0 = arg0->m_pNeighbours[0];
            arg0->m_wFlags = (u16) (temp_v1 | 4);
            if (temp_a0 != NULL) {
                CGameRegion__FloodSetFlag4Same(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodSetFlag4Same(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodSetFlag4Same(temp_a0_3, temp_s0);
            }
        }
    }
}
