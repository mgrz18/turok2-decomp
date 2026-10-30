/* NON-MATCHING: 1 of 62 words differ: the j at 0x26C184 (region NULL or flag clear) goes to the info == NULL test at 0x26C1C4 with v0 = 0, where the ROM threads it straight to the epilogue at 0x26C224. match_func masks a local j target, so it reports MATCH; make verify does not. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;
} Region;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 height;
} RegionInfo;

extern u8 D_800F7078[];
extern Region *D_800B2610;     /* region of the cached info */
extern RegionInfo *D_800B2614; /* its cached info */

extern RegionInfo *func_0022425C(void *table, Region *region);

/* Whether the segment from a to b crosses the height of the region's surface,
 * strictly between its ends; *t gets the crossing fraction. */
s32 func_0026C148(Region *region, Vec3f *a, Vec3f *b, f32 *t) {
    RegionInfo *info;
    f32 dy;
    f32 f;
    s32 result;

    if (region == NULL || !(region->flags & 1)) {
        info = NULL;
    } else {
        if (region != D_800B2610) {
            D_800B2614 = func_0022425C(D_800F7078, region);
            D_800B2610 = region;
        }
        info = D_800B2614;
    }
    if (info == NULL) {
        return 0;
    }
    dy = b->y - a->y;
    if (dy != 0.0f) {
        result = 0;
        f = (info->height - a->y) / dy;
        *t = f;
        if (f > 0.0f && f < 1.0f) {
            result = 1;
        }
        return result;
    }
    return 0;
}
