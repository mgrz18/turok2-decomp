#include "common.h"

typedef struct {
    /* 0x0 */ u16 id;
    /* 0x2 */ u16 flags;
} Region;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 unk20;
} RegionInfo;

/* An instance header: its current region at 0x10. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Region *region;
} Instance;

RegionInfo *func_0022425C(s32 *, Region *);             /* extern */
extern s32 D_800F7078;

f32 func_00268A5C(Instance *arg0) {
    Region *temp_a1;
    RegionInfo *temp_v0;

    temp_a1 = arg0->region;
    if ((temp_a1 != NULL) && (temp_a1->flags & 1)) {
        temp_v0 = func_0022425C(&D_800F7078, temp_a1);
        if (temp_v0 != NULL) {
            return temp_v0->unk20;
        }
    }
    return 0.0f;
}
