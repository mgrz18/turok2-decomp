#include "common.h"
#include "structs/vector.h"

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
 * strictly between its ends; *t gets the crossing fraction.
 * The failure paths share the first `return 0` through a label: the ROM's
 * single `v0 = 0; j epilogue` block sits right after the region test, where
 * three separate returns would be cross-jumped into the last one instead. */
s32 func_0026C148(Region *region, CVector3 *a, CVector3 *b, f32 *t) {
    RegionInfo *info;
    f32 dy;
    f32 f;
    s32 result;

    if (region == NULL || !(region->flags & 1)) {
    fail:
        return 0;
    }
    if (region != D_800B2610) {
        D_800B2614 = func_0022425C(D_800F7078, region);
        D_800B2610 = region;
    }
    info = D_800B2614;
    if (info == NULL) {
        goto fail;
    }
    dy = b->y - a->y;
    if (dy == 0.0f) {
        goto fail;
    }
    result = 0;
    f = (info->height - a->y) / dy;
    *t = f;
    if (f > 0.0f && f < 1.0f) {
        result = 1;
    }
    return result;
}
