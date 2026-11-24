#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/collision.h"
#include "globals/ai.h"
#include "globals/path.h"

extern u8 D_800C894C;                   /* bottom of the stack */
#ifdef TARGET_PORT
/* the N64 compares the stack pointer with its main stack's bottom; the host asks its own thread's
 * stack (t2os_stack_low, port/runtime/t2os.c: the address comparison depended on the layout) */
int t2os_stack_low(const void *local);
#define T2_STACK_LOW(local, bottom) t2os_stack_low(local)
#else
#define T2_STACK_LOW(local, bottom) ((uptr) (local) < (uptr) (bottom))
#endif

f32 CGameRegion__GetCentreDistance(CGameRegion *from, CGameRegion *to);
void CGameRegion__GetCentroid(CGameRegion *region, CVector3 *centre);
sptr Sweep_CrossRegionEdge(void *mover, CGameRegion *region, s32 edge, CVector3 *centre);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
#define IN_BOUNDS(p) ((p)->x >= sPathSearch->bounds[0] && (p)->x <= sPathSearch->bounds[2] \
                      && (p)->z >= sPathSearch->bounds[1] && (p)->z <= sPathSearch->bounds[3])

/* Walks the region graph from region, depth first and nearest neighbour
 * first, appending a node for each step and keeping in path the cheapest
 * node that reached the start region. */
void Path_ExpandNode(PathNode *parent, CGameRegion *region) {
    s8 next[3];
    PathNode *node;

    if (T2_STACK_LOW(&parent, &D_800C894C)) {
        return;
    }
    if (sPathSearch->nodeCount >= 150) {
        return;
    }
    if (!(IN_BOUNDS(region->m_pVertices[0]) || IN_BOUNDS(region->m_pVertices[1]) || IN_BOUNDS(region->m_pVertices[2]))) {
        return;
    }
    if (parent != NULL) {
        sPathCost = parent->cost + CGameRegion__GetCentreDistance(parent->region, region);
        sPathDepth = parent->count + 1;
        if (sPathSearch->path != NULL && sPathSearch->path->cost <= sPathCost) {
            return;
        }
    } else {
        sPathCost = 0.0f;
        sPathDepth = 0;
    }
    node = sPathSearch->poolNext;
    sPathSearch->poolNext = node + 1;
    sPathSearch->nodeCount++;
    node->region = region;
    node->parent = parent;
    node->count = sPathDepth;
    node->cost = sPathCost;
    if (region == sPathSearch->startRegion) {
        if (sPathSearch->path == NULL || sPathCost < sPathSearch->path->cost) {
            sPathSearch->path = node;
        }
        return;
    }
    sPathCandidateCount = 0;
    sPathEdge = 0;
    region->m_wFlags |= 0x8000;
    do {
        sPathNeighbour = region->m_pNeighbours[sPathEdge];
        if (sPathNeighbour != NULL && !(sPathNeighbour->m_wFlags & 0x8000)) {
            sPathFound = 0;
            for (sPathBackEdge = 0; sPathBackEdge < 3; sPathBackEdge++) {
                if (sPathNeighbour->m_pNeighbours[sPathBackEdge] == region) {
                    sPathFound = 1;
                    break;
                }
            }
            if (sPathFound) {
                CGameRegion__GetCentroid(sPathNeighbour, &sPathCentre);
                sPathCrossed = Sweep_CrossRegionEdge(&sPathSearch->mover, sPathNeighbour, sPathBackEdge, &sPathCentre);
                if (sPathCrossed) {
                    CVector3__Subtract(&sPathCentre, &sPathCentre, &sPathSearch->startPos);
                    sPathCost = sPathCentre.x * sPathCentre.x + sPathCentre.y * sPathCentre.y
                               + sPathCentre.z * sPathCentre.z;
                    sPathCandidates[sPathCandidateCount].edge = sPathEdge;
                    sPathCandidates[sPathCandidateCount++].dist = sPathCost;
                }
            }
        }
        sPathEdge++;
    } while (sPathEdge < 3);

    switch (sPathCandidateCount) {
        case 0:
            return;
        case 1:
            next[1] = -1;
            next[2] = -1;
            next[0] = sPathCandidates[0].edge;
            break;
        case 2:
            if (sPathCandidates[0].dist < sPathCandidates[1].dist) {
                next[0] = sPathCandidates[0].edge;
                next[1] = sPathCandidates[1].edge;
            } else {
                next[0] = sPathCandidates[1].edge;
                next[1] = sPathCandidates[0].edge;
            }
            next[2] = -1;
            break;
        case 3:
            sPathSorted[0] = &sPathCandidates[0];
            sPathSorted[1] = &sPathCandidates[1];
            sPathSorted[2] = &sPathCandidates[2];
            if (sPathSorted[0]->dist > sPathSorted[1]->dist) {
                sPathSwap = sPathSorted[0];
                sPathSorted[0] = sPathSorted[1];
                sPathSorted[1] = sPathSwap;
            }
            if (sPathSorted[1]->dist > sPathSorted[2]->dist) {
                sPathSwap = sPathSorted[1];
                sPathSorted[1] = sPathSorted[2];
                sPathSorted[2] = sPathSwap;
            }
            if (sPathSorted[0]->dist > sPathSorted[1]->dist) {
                sPathSwap = sPathSorted[0];
                sPathSorted[0] = sPathSorted[1];
                sPathSorted[1] = sPathSwap;
            }
            next[0] = sPathSorted[0]->edge;
            next[1] = sPathSorted[1]->edge;
            next[2] = sPathSorted[2]->edge;
            break;
    }
    if (next[0] != -1) {
        Path_ExpandNode(node, region->m_pNeighbours[next[0]]);
    }
    if (next[1] != -1) {
        Path_ExpandNode(node, region->m_pNeighbours[next[1]]);
    }
    if (next[2] != -1) {
        Path_ExpandNode(node, region->m_pNeighbours[next[2]]);
    }
}

#include "globals/engine.h"

void BoundingRect_AddPoints(f32 *bounds, s32 count, CVector3 *points);
void CGameRegion__ClearVisited(CGameRegion *region);

/* Searches a path of regions from obj to goalRegion (Path_ExpandNode,
 * CGameRegion__ClearVisited) and puts in *out the centre of the region to head for.
 * Returns 0 if no path was found. */
s32 Path_FindRegionPath(CGameObjectInstance *obj, CVector3 goal, CGameRegion *goalRegion, sptr arg, CVector3 *out) {
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

s32 GameObject_CanMoveTo(CInstanceHeader *inst, CVector3 pos, sptr arg);

/* Picks the neighbouring region whose centre is farthest from pos and still
 * passes GameObject_CanMoveTo; its centre goes to *out. */
CGameRegion *Path_FindFleeRegion(CInstanceHeader *inst, CGameRegion *exclude, CVector3 pos, CGameRegion *none,
                      sptr arg, CVector3 *out) {
    CGameRegion *cur;
    CGameRegion *region;
    CGameRegion *best;
    CVector3 centre;
    CVector3 delta;
    f32 bestDist;
    f32 dist;
    s32 skipExcluded;
    s32 i;

    *out = pos;
    cur = (CGameRegion *)inst->m_pCurrentRegion;
    if (cur == NULL) {
        return none;
    }
    best = NULL;
    bestDist = 0.0f;
    skipExcluded = gFrameCountGameplay >> 4;
    skipExcluded &= 1;
    for (i = 0; i < 3; i++) {
        region = cur->m_pNeighbours[i];
        if (region == NULL) {
            continue;
        }
        if (skipExcluded && exclude == region) {
            continue;
        }
        CGameRegion__GetCentroid(region, &centre);
        CVector3__Subtract(&delta, &centre, &pos);
        dist = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (bestDist < dist && GameObject_CanMoveTo(inst, centre, arg) != 0) {
            bestDist = dist;
            best = region;
            *out = centre;
        }
    }
    return best;
}

f32 GameObject_GetSweepRadius(sptr);
void Sphere_Set(Sphere *sphere, CVector3 *center, f32 radius);
s32 Cylinder_ContainsPoint(Sphere *sphere, CVector3 *point);
s32 Circle_SegmentIntersects(Sphere *sphere, CVector3 *a, CVector3 *b);

/* Whether a mover at pos fits in region: for each edge not shared with prev
 * or goal and not passable (Sweep_CrossRegionEdge), the mover's circle must not
 * touch either corner or the edge itself. */
s32 Path_FitsInRegion(s32 *mover, CGameRegion *region, CVector3 *pos, CGameRegion *prev, CGameRegion *goal) {
    Sphere sphere;
    CVector3 *a;
    CVector3 *b;
    s32 i;

    if (region == prev || region == goal) {
        return 1;
    }
    Sphere_Set(&sphere, pos, GameObject_GetSweepRadius(*mover) * 0.5f);
    for (i = 0; i < 3; i++) {
        if (region->m_pNeighbours[i] == prev) {
            continue;
        }
        if (region->m_pNeighbours[i] == goal) {
            continue;
        }
        if (Sweep_CrossRegionEdge(mover, region, i, pos) != 0) {
            continue;
        }
        a = region->m_pVertices[i];
        b = region->m_pVertices[i == 2 ? 0 : i + 1];
        if (Cylinder_ContainsPoint(&sphere, a) || Cylinder_ContainsPoint(&sphere, b)) {
            return 0;
        }
        if (Circle_SegmentIntersects(&sphere, a, b)) {
            return 0;
        }
    }
    return 1;
}
