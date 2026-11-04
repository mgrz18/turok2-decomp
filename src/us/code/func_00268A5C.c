#include "common.h"
#include "structs/region.h"

/* An instance header: its current region at 0x10. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ CGameRegion *region;
} Instance;

CRegionInfo *func_0022425C(s32 *, CGameRegion *);             /* extern */
extern s32 D_800F7078;

f32 func_00268A5C(Instance *arg0) {
    CGameRegion *temp_a1;
    CRegionInfo *temp_v0;

    temp_a1 = arg0->region;
    if ((temp_a1 != NULL) && (temp_a1->m_wFlags & 1)) {
        temp_v0 = func_0022425C(&D_800F7078, temp_a1);
        if (temp_v0 != NULL) {
            return temp_v0->m_WaterHeight;
        }
    }
    return 0.0f;
}
