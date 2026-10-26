#include "common.h"
#include "structs/game_object.h"

/* A navigation region: a triangle with its three corners and the regions
 * across each edge. */
typedef struct Region {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ CVector3 *corner[3];
    /* 0x10 */ struct Region *neighbour[3];
} Region;

/* One step of the path found, a list node. */
typedef struct PathNode {
    /* 0x00 */ Region *region;
    /* 0x04 */ struct PathNode *next;
    /* 0x08 */ s32 count;
} PathNode;

/* The path search state, kept on the stack and reached through D_800F1A48. */
typedef struct PathSearch {
    /* 0x000 */ CGameObjectInstance *obj;
    /* 0x004 */ Region *startRegion;
    /* 0x008 */ CVector3 startPos;
    /* 0x014 */ Region *goalRegion;
    /* 0x018 */ CVector3 goalPos;
    /* 0x024 */ f32 bounds[4];          /* min x, min z, max x, max z */
    /* 0x034 */ u8 pool[0x960];
    /* 0x994 */ u8 *poolNext;
    /* 0x998 */ s32 unk998;
    /* 0x99C */ PathNode *path;
    /* 0x9A0 */ CGameObjectInstance *obj2;
    /* 0x9A4 */ s32 arg;
    /* 0x9A8 */ u8 pad9A8[0x44];
    /* 0x9EC */ CVector3 goalPos2;
    /* 0x9F8 */ u8 pad9F8[0x44];
    /* 0xA3C */ s32 unkA3C;
    /* 0xA40 */ s32 unkA40;
    /* 0xA44 */ s32 unkA44;
} PathSearch; /* size 0xA48 */

extern u32 D_800B6D20;
extern PathSearch *D_800F1A48;

void func_0026CB2C(Region *region, CVector3 *centre);
void func_0026CDAC(f32 *bounds, s32 count, CVector3 *points);
void func_0026CC0C(Region *region);
s32 func_002393D0(s32, Region *);

/* Searches a path of regions from obj to goalRegion (func_002393D0,
 * func_0026CC0C) and puts in *out the centre of the region to head for.
 * Returns 0 if no path was found. */
s32 func_002399F8(CGameObjectInstance *obj, CVector3 goal, Region *goalRegion, s32 arg, CVector3 *out) {
    PathSearch search;
    Region *region;
    PathNode *node;
    CVector3 pos;
    s32 i;
    PathSearch *s;

    *out = goal;
    if (obj->m_pInstanceBelow != NULL) {
        return 1;
    }
    region = obj->header.m_pCurrentRegion;
    do {
        if (region == NULL || goalRegion == NULL || region == goalRegion) {
            return 1;
        }
        i = 0;
    } while (0);
    s = &search;
    D_800F1A48 = s;
    pos = obj->header.m_vPos;
    search.obj = obj;
    s->startRegion = region;
    search.startPos = pos;
    D_800F1A48->goalRegion = goalRegion;
    D_800F1A48->goalPos = goal;
    D_800F1A48->poolNext = D_800F1A48->pool;
    D_800F1A48->unk998 = 0;
    D_800F1A48->path = NULL;
    D_800F1A48->bounds[0] = obj->header.m_vPos.x - 5120.0f;
    D_800F1A48->bounds[2] = obj->header.m_vPos.x + 5120.0f;
    D_800F1A48->bounds[1] = obj->header.m_vPos.z - 5120.0f;
    D_800F1A48->bounds[3] = obj->header.m_vPos.z + 5120.0f;
    do {
        func_0026CDAC(D_800F1A48->bounds, 1, region->corner[i]);
        i++;
    } while (i < 3);
    D_800F1A48->obj2 = obj;
    D_800F1A48->arg = arg;
    D_800F1A48->goalPos2 = goal;
    D_800F1A48->unkA3C = 0;
    D_800F1A48->unkA40 = 1;
    func_002393D0(0, D_800F1A48->goalRegion);
    func_0026CC0C(D_800F1A48->goalRegion);
    node = D_800F1A48->path;
    if (node == NULL) {
        return 0;
    }
    if (node->count >= 3) {
        i = (D_800B6D20 >> 4) & 1;
        for (; i != -1; i--) {
            if (node->next != NULL) {
                node = node->next;
            }
        }
        func_0026CB2C(node->region, out);
    }
    return 1;
}
