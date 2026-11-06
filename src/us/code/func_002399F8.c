#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "globals/engine.h"
#include "globals/ai.h"
#include "globals/path.h"

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
    sPathSearch->nodeCount = 0;
    sPathSearch->path = NULL;
    sPathSearch->bounds[0] = obj->header.m_vPos.x - 5120.0f;
    sPathSearch->bounds[2] = obj->header.m_vPos.x + 5120.0f;
    sPathSearch->bounds[1] = obj->header.m_vPos.z - 5120.0f;
    sPathSearch->bounds[3] = obj->header.m_vPos.z + 5120.0f;
    do {
        BoundingRect_AddPoints(sPathSearch->bounds, 1, region->m_pVertices[i]);
        i++;
    } while (i < 3);
    sPathSearch->mover.obj = obj;
    sPathSearch->mover.def = (CCollisionInfo *) arg;
    sPathSearch->mover.end = goal;
    sPathSearch->mover.stop = 0;
    sPathSearch->mover.unkA0 = 1;
    Path_ExpandNode(0, sPathSearch->goalRegion);
    CGameRegion__ClearVisited(sPathSearch->goalRegion);
    node = sPathSearch->path;
    if (node == NULL) {
        return 0;
    }
    if (node->count >= 3) {
        i = (gFrameCountGameplay >> 4) & 1;
        for (; i != -1; i--) {
            if (node->parent != NULL) {
                node = node->parent;
            }
        }
        CGameRegion__GetCentroid(node->region, out);
    }
    return 1;
}
