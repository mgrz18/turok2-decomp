#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"

/* One step of a path, a node in the search's pool. */
typedef struct PathNode {
    /* 0x00 */ CGameRegion *region;
    /* 0x04 */ struct PathNode *parent;
    /* 0x08 */ s32 count;
    /* 0x0C */ f32 cost;
} PathNode;

/* The path search state (see Path_FindRegionPath). */
typedef struct PathSearch {
    /* 0x000 */ CGameObjectInstance *obj;
    /* 0x004 */ CGameRegion *startRegion;
    /* 0x008 */ CVector3 startPos;
    /* 0x014 */ CGameRegion *goalRegion;
    /* 0x018 */ CVector3 goalPos;
    /* 0x024 */ f32 bounds[4];          /* min x, min z, max x, max z */
    /* 0x034 */ u8 pool[0x960];
    /* 0x994 */ PathNode *poolNext;
    /* 0x998 */ s32 nodeCount;
    /* 0x99C */ PathNode *path;
    /* 0x9A0 */ u8 mover[0xA8];         /* who moves, for Sweep_CrossRegionEdge */
} PathSearch;

/* A neighbour worth exploring: its edge index and squared distance. */
typedef struct Candidate {
    /* 0x0 */ s32 edge;
    /* 0x4 */ f32 dist;
} Candidate;

extern PathSearch *sPathSearch;
extern u8 D_800C894C;                   /* bottom of the stack */

/* The search's working variables are globals, to keep the recursion's
 * frames small. */
extern CGameRegion *sPathNeighbour;              /* neighbour */
extern Candidate sPathCandidates[3];         /* candidates */
extern Candidate *sPathSorted[3];        /* candidates sorted by distance */
extern Candidate *sPathSwap;           /* swap */
extern s32 sPathCandidateCount;                  /* candidate count */
extern s32 sPathEdge;                  /* edge */
extern s32 sPathDepth;                  /* node count */
extern s32 sPathBackEdge;                  /* neighbour's edge back */
extern CVector3 sPathCentre;             /* neighbour's centre */
extern f32 sPathCost;                  /* cost */
extern s32 sPathFound;                  /* found */
extern s32 sPathCrossed;                  /* passable */

f32 CGameRegion__GetCentreDistance(CGameRegion *from, CGameRegion *to);
void CGameRegion__GetCentroid(CGameRegion *region, CVector3 *centre);
s32 Sweep_CrossRegionEdge(void *mover, CGameRegion *region, s32 edge, CVector3 *centre);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);

#define IN_BOUNDS(p) ((p)->x >= sPathSearch->bounds[0] && (p)->x <= sPathSearch->bounds[2] \
                      && (p)->z >= sPathSearch->bounds[1] && (p)->z <= sPathSearch->bounds[3])

/* Walks the region graph from region, depth first and nearest neighbour
 * first, appending a node for each step and keeping in path the cheapest
 * node that reached the start region. */
void Path_ExpandNode(PathNode *parent, CGameRegion *region) {
    s8 next[3];
    PathNode *node;

    if ((u32) &parent < (u32) &D_800C894C) {
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
                sPathCrossed = Sweep_CrossRegionEdge(sPathSearch->mover, sPathNeighbour, sPathBackEdge, &sPathCentre);
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
