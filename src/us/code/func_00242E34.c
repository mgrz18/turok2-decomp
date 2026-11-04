#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

typedef struct Region {
    char pad0[2];
    u16 flags;         /* 0x02 */
} Region;

typedef struct RegionInfo {
    char pad0[0x28];
    f32 interval;      /* 0x28 */
    char pad2C[0x44 - 0x2C];
    u32 flags;         /* 0x44 */
    char pad48[0x50 - 0x48];
    u16 damage;        /* 0x50 */
} RegionInfo;

typedef struct Owner {
    char pad0[0x51C];
    char *player;      /* 0x51C */
} Owner;


typedef struct Dest {
    u32 flags;         /* 0x00 */
    char pad4[0xCC - 0x4];
    f32 timer;         /* 0xCC */
} Dest;

typedef struct Damage {
    char pad[0x18];
} Damage;

extern char D_800F7078[];
extern f32 D_800B6D28;  /* frame time scale */

RegionInfo *func_0022425C(void *world, Region *region);
f32 func_002671B4(CGameObjectInstance *actor);
f32 func_00268A5C(CGameObjectInstance *actor);
s32 func_0026AFC0(Region *region);
s32 func_0021F080(CGameObjectInstance *actor);
f32 func_0026BE60(Region *region, f32 x, f32 z);
s32 func_002685F0(CGameObjectInstance *actor);
void func_00245828(Damage *damage, s32 arg1, s32 amount, f32 force, s32 kind, s32 arg5);
void func_002465C8(CGameObjectInstance *actor, Dest *dst, Damage *damage);

/* Region flags for the actor (water, ground...) and periodic damage from the region it stands in. */
void func_00242E34(CGameObjectInstance *actor, Dest *dst) {
    Region *region;
    RegionInfo *info;
    Damage damage;
    f32 y;
    f32 top;
    s32 hurt;
    s32 kind;

    dst->flags &= ~0x7F80;
    region = ((Region *)actor->header.m_pCurrentRegion);
    if (region == NULL) {
        return;
    }
    info = func_0022425C(D_800F7078, region);
    if (info == NULL) {
        return;
    }
    if (info->flags & 0x1000000) {
        dst->flags |= 0x800;
    }
    if (region->flags & 1) {
        if (info->flags & 0x20000000) {
            dst->flags |= 0x400;
        } else {
            if (actor->m_dwFlags & 0x300000) {
                y = actor->header.m_vPos.y;
                top = *(f32 *)(((Owner *)actor->m_AI.m_pParent)->player + 0x250);
            } else {
                top = func_002671B4(actor) + actor->header.m_vPos.y;
                y = actor->header.m_vPos.y;
            }
            if (top <= func_00268A5C(actor)) {
                dst->flags |= 0x80;
            } else if (func_00268A5C(actor) < y) {
                dst->flags |= 0x100;
            } else {
                dst->flags |= 0x200;
            }
        }
    }
    if (func_0026AFC0(region) && (region->flags & 0x10) && func_0021F080(actor)) {
        dst->flags |= 0x1000;
    }
    if (func_0026BE60(region, actor->header.m_vPos.x, actor->header.m_vPos.z) - actor->header.m_vPos.y < 15.36f) {
        if (region->flags & 0x100) {
            dst->flags |= 0x2000;
        }
        if (region->flags & 0x80) {
            dst->flags |= 0x4000;
        }
    }
    if (info->flags & 0x80000) {
        hurt = 1;
        if (info->flags & 4) {
            hurt = actor->header.m_pIntelligence->m_Type != 1;
        }
        if (dst->flags & 0xC0000) {
            hurt = 0;
        }
        if (!func_002685F0(actor)) {
            hurt = 0;
        }
        if (actor->m_pInstanceBelow != NULL) {
            hurt = 0;
        }
        if (hurt) {
            if (!(dst->flags & 0x8000)) {
                dst->timer += D_800B6D28 * (1.0f / 15);
                if (dst->timer >= info->interval) {
                    kind = 0x100000;
                    if (info->flags & 0x400000) {
                        kind = 0x1100000;
                    }
                    dst->timer -= info->interval;
                    func_00245828(&damage, 0, info->damage << 8, 25.599998f, kind, 0);
                    func_002465C8(actor, dst, &damage);
                }
            }
            if (dst->flags & 0x8000) {
                dst->timer = info->interval;
                dst->flags &= ~0x8000;
            }
        } else {
            dst->flags &= ~0x8000;
        }
    } else {
        dst->flags |= 0x8000;
    }
}
