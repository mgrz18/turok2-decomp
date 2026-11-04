/* Cut fixed in versions/cuts.us.txt: func_00268AB4 really is 0x268AB4-0x268B50 (0x9C bytes): splat split off its epilogue (jr ra; addiu sp) as func_00268B48. */
#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x250];
    /* 0x250 */ f32 height;
} OwnerData;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ OwnerData *data;
} Owner;

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;
} Region;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 height;
} RegionInfo;


extern RegionInfo *func_0022425C(void *table, Region *region);
extern u8 D_800F7078[];

/* Height of the actor's region (0 if it has none), relative to the owner's
 * height for a kind-1 actor flagged 0x300000. */
f32 func_00268AB4(CGameObjectInstance *actor) {
    Region *region;
    RegionInfo *info;
    f32 h;

    region = ((Region *)actor->header.m_pCurrentRegion);
    if (region == NULL || !(region->flags & 1)
        || (info = func_0022425C(D_800F7078, region)) == NULL) {
        return 0.0f;
    }
    h = info->height;
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        h -= ((Owner *)actor->m_AI.m_pParent)->data->height;
    }
    return h;
}
