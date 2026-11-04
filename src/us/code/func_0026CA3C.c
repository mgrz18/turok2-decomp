#include "common.h"
#include "structs/region.h"

/* Sets the height of arg0's vertices to arg1 and spreads it to every node
 * with id arg2 linked to it. */
void func_0026CA3C(CGameRegion *arg0, f32 arg1, s32 arg2) {
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    if (arg0->m_nInfo == (arg2 & 0xFFFF)) {
        if (arg0->m_pVertices[0]->y == arg1) {
            if ((arg0->m_pVertices[1]->y != arg1) || (arg0->m_pVertices[2]->y != arg1)) {
                goto block_5;
            }
        } else {
block_5:
            arg0->m_pVertices[0]->y = arg1;
            arg0->m_pVertices[1]->y = arg1;
            arg0->m_pVertices[2]->y = arg1;
            temp_a0 = arg0->m_pNeighbours[0];
            if (temp_a0 != NULL) {
                func_0026CA3C(temp_a0, arg1, arg2 & 0xFFFF);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                func_0026CA3C(temp_a0_2, arg1, arg2 & 0xFFFF);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                func_0026CA3C(temp_a0_3, arg1, arg2 & 0xFFFF);
            }
        }
    }
}
