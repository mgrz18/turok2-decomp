#include "common.h"
#include "globals/scene.h"
#include "structs/region.h"
#include "protos/scene.h"
#include "protos/region.h"

/* Resource entry: a key, its flags and three child entries. */



void CGameRegion__FloodSetWater(CGameRegion *arg0) {
    sptr temp_a0;
    sptr temp_a0_2;
    sptr temp_a0_3;
    CRegionInfo *temp_v0;
    s32 temp_s0;
    u16 temp_v1;

    if (arg0 != NULL) {
        temp_v1 = arg0->m_wFlags;
        if (!(temp_v1 & 1)) {
            temp_a0 = (sptr) arg0->m_pNeighbours[0];
            temp_s0 = arg0->m_nInfo;
            arg0->m_wFlags = (u16) (temp_v1 | 1);
            if (temp_a0 != 0) {
                CGameRegion__FloodSetWaterSame((CGameRegion *)temp_a0, temp_s0);
            }
            temp_a0_2 = (sptr) arg0->m_pNeighbours[1];
            if (temp_a0_2 != 0) {
                CGameRegion__FloodSetWaterSame((CGameRegion *)temp_a0_2, temp_s0);
            }
            temp_a0_3 = (sptr) arg0->m_pNeighbours[2];
            if (temp_a0_3 != 0) {
                CGameRegion__FloodSetWaterSame((CGameRegion *)temp_a0_3, temp_s0);
            }
        }
        temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
        if (temp_v0 != NULL) {
            temp_v0->m_MapColour = 2;
            temp_v0->m_dwFlags = temp_v0->m_dwFlags | 1;
            temp_v0->unk_5B = (u8) (temp_v0->unk_5B | 2);
        }
    }
}
