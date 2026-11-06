#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "globals/engine.h"
#include "globals/ai.h"

/* One step of the path found, a list node. */
typedef struct PathNode {
    /* 0x00 */ CGameRegion *region;
    /* 0x04 */ struct PathNode *next;
    /* 0x08 */ s32 count;
} PathNode;

/* The path search state, kept on the stack and reached through sPathSearch. */
typedef struct PathSearch {
    /* 0x000 */ CGameObjectInstance *obj;
    /* 0x004 */ CGameRegion *startRegion;
    /* 0x008 */ CVector3 startPos;
    /* 0x014 */ CGameRegion *goalRegion;
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


void CGameRegion__GetCentroid(CGameRegion *region, CVector3 *centre);
void BoundingRect_AddPoints(f32 *bounds, s32 count, CVector3 *points);
void CGameRegion__ClearVisited(CGameRegion *region);
s32 Path_ExpandNode(s32, CGameRegion *);

/* Searches a path of regions from obj to goalRegion (Path_ExpandNode,
 * CGameRegion__ClearVisited) and puts in *out the centre of the region to head for.
 * Returns 0 if no path was found. */
s32 Path_FindRegionPath(CGameObjectInstance *obj, CVector3 goal, CGameRegion *goalRegion, s32 arg, CVector3 *out) {
    PathSearch search;
    CGameRegion *region;
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
    sPathSearch = s;
    pos = obj->header.m_vPos;
    search.obj = obj;
    s->startRegion = region;
    search.startPos = pos;
    sPathSearch->goalRegion = goalRegion;
    sPathSearch->goalPos = goal;
    sPathSearch->poolNext = sPathSearch->pool;
    sPathSearch->unk998 = 0;
    sPathSearch->path = NULL;
    sPathSearch->bounds[0] = obj->header.m_vPos.x - 5120.0f;
    sPathSearch->bounds[2] = obj->header.m_vPos.x + 5120.0f;
    sPathSearch->bounds[1] = obj->header.m_vPos.z - 5120.0f;
    sPathSearch->bounds[3] = obj->header.m_vPos.z + 5120.0f;
    do {
        BoundingRect_AddPoints(sPathSearch->bounds, 1, region->m_pVertices[i]);
        i++;
    } while (i < 3);
    sPathSearch->obj2 = obj;
    sPathSearch->arg = arg;
    sPathSearch->goalPos2 = goal;
    sPathSearch->unkA3C = 0;
    sPathSearch->unkA40 = 1;
    Path_ExpandNode(0, sPathSearch->goalRegion);
    CGameRegion__ClearVisited(sPathSearch->goalRegion);
    node = sPathSearch->path;
    if (node == NULL) {
        return 0;
    }
    if (node->count >= 3) {
        i = (gFrameCountGameplay >> 4) & 1;
        for (; i != -1; i--) {
            if (node->next != NULL) {
                node = node->next;
            }
        }
        CGameRegion__GetCentroid(node->region, out);
    }
    return 1;
}
