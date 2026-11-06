#include "common.h"
#include "structs/region.h"

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ CGameRegion *node;
} Arg0;

s32 Automap_RevealRegion(s32);
s32 Automap_ClearVisited(s32);

void Automap_RevealAroundPlayer(Arg0 *arg0) {
    CGameRegion *temp_s0;
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;

    Automap_RevealRegion((s32) arg0->node);
    temp_s0 = arg0->node;
    if (temp_s0 != 0) {
        temp_a0 = temp_s0->m_pNeighbours[0];
        temp_s0->m_wFlags = (u16) (temp_s0->m_wFlags & 0x7FFF);
        if ((temp_a0 != NULL) && (temp_a0->m_wFlags & 0x8000)) {
            Automap_ClearVisited((s32) temp_a0);
        }
        temp_a0_2 = temp_s0->m_pNeighbours[1];
        if ((temp_a0_2 != NULL) && (temp_a0_2->m_wFlags & 0x8000)) {
            Automap_ClearVisited((s32) temp_a0_2);
        }
        temp_a0_3 = temp_s0->m_pNeighbours[2];
        if ((temp_a0_3 != NULL) && (temp_a0_3->m_wFlags & 0x8000)) {
            Automap_ClearVisited((s32) temp_a0_3);
        }
    }
}
