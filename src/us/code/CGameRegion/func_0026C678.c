#include "common.h"
#include "structs/region.h"
#include "globals/scene.h"
#include "protos/scene.h"

void CGameRegion__FloodClearWaterAll(CGameRegion *);                  /* extern */

void CGameRegion__FloodClearWater(CGameRegion *arg0) {
    CGameRegion *temp_a0;
    CGameRegion *temp_a0_2;
    CGameRegion *temp_a0_3;
    u16 temp_v1;
    CRegionInfo *temp_v0;

    if (arg0 != NULL) {
        temp_v1 = arg0->m_wFlags;
        if (temp_v1 & 1) {
            temp_a0 = arg0->m_pNeighbours[0];
            arg0->m_wFlags = (u16) (temp_v1 & 0xFFFE);
            if (temp_a0 != NULL) {
                CGameRegion__FloodClearWaterAll(temp_a0);
            }
            temp_a0_2 = arg0->m_pNeighbours[1];
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodClearWaterAll(temp_a0_2);
            }
            temp_a0_3 = arg0->m_pNeighbours[2];
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodClearWaterAll(temp_a0_3);
            }
        }
        temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
        if (temp_v0 != NULL) {
            temp_v0->unk_58 = 0;
            temp_v0->m_MapColour = 0;
            temp_v0->m_dwFlags = (s32) (temp_v0->m_dwFlags & ~1);
            temp_v0->unk_5B = (u8) (temp_v0->unk_5B & 0xFD);
        }
    }
}
