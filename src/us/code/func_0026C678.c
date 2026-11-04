#include "common.h"
#include "structs/region.h"

CRegionInfo *func_0022425C(void *, void *);          /* extern */
void func_0026CEC8(CGameRegion *);                  /* extern */
extern u8 D_800F7078[];

void func_0026C678(CGameRegion *arg0) {
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
        temp_v0 = func_0022425C(D_800F7078, arg0);
        if (temp_v0 != NULL) {
            temp_v0->unk_58 = 0;
            temp_v0->m_MapColour = 0;
            temp_v0->m_dwFlags = (s32) (temp_v0->m_dwFlags & ~1);
            temp_v0->unk_5B = (u8) (temp_v0->unk_5B & 0xFD);
        }
    }
}
