#include "common.h"
#include "globals/scene.h"
#include "structs/region.h"
#include "protos/scene.h"

/* Resource entry: a key, its flags and three child entries. */
typedef struct Entry0026C5C4 {
    /* 0x00 */ u16 key;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ struct Entry0026C5C4 *child0;
    /* 0x14 */ struct Entry0026C5C4 *child1;
    /* 0x18 */ struct Entry0026C5C4 *child2;
} Entry0026C5C4;


s32 CGameRegion__FloodSetWaterSame(sptr, s32);                    /* extern */

void CGameRegion__FloodSetWater(Entry0026C5C4 *arg0) {
    sptr temp_a0;
    sptr temp_a0_2;
    sptr temp_a0_3;
    CRegionInfo *temp_v0;
    s32 temp_s0;
    u16 temp_v1;

    if (arg0 != NULL) {
        temp_v1 = arg0->flags;
        if (!(temp_v1 & 1)) {
            temp_a0 = (sptr) arg0->child0;
            temp_s0 = arg0->key;
            arg0->flags = (u16) (temp_v1 | 1);
            if (temp_a0 != 0) {
                CGameRegion__FloodSetWaterSame(temp_a0, temp_s0);
            }
            temp_a0_2 = (sptr) arg0->child1;
            if (temp_a0_2 != 0) {
                CGameRegion__FloodSetWaterSame(temp_a0_2, temp_s0);
            }
            temp_a0_3 = (sptr) arg0->child2;
            if (temp_a0_3 != 0) {
                CGameRegion__FloodSetWaterSame(temp_a0_3, temp_s0);
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
