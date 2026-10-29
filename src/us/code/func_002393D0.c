#include "common.h"
#include "structs/game_object.h"

/* A navigation region: a triangle with its three corners and the regions
 * across each edge. */
typedef struct Region {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;               /* 0x8000: visited by the search */
    /* 0x04 */ CVector3 *corner[3];
    /* 0x10 */ struct Region *neighbour[3];
} Region;

/* One step of a path, a node in the search's pool. */
typedef struct PathNode {
    /* 0x00 */ Region *region;
    /* 0x04 */ struct PathNode *parent;
    /* 0x08 */ s32 count;
    /* 0x0C */ f32 cost;
} PathNode;

/* The path search state (see func_002399F8). */
typedef struct PathSearch {
    /* 0x000 */ CGameObjectInstance *obj;
    /* 0x004 */ Region *startRegion;
    /* 0x008 */ CVector3 startPos;
    /* 0x014 */ Region *goalRegion;
    /* 0x018 */ CVector3 goalPos;
    /* 0x024 */ f32 bounds[4];          /* min x, min z, max x, max z */
    /* 0x034 */ u8 pool[0x960];
    /* 0x994 */ PathNode *poolNext;
    /* 0x998 */ s32 nodeCount;
    /* 0x99C */ PathNode *path;
    /* 0x9A0 */ u8 mover[0xA8];         /* who moves, for func_0023B664 */
} PathSearch;

/* A neighbour worth exploring: its edge index and squared distance. */
typedef struct Candidate {
    /* 0x0 */ s32 edge;
    /* 0x4 */ f32 dist;
} Candidate;

extern PathSearch *D_800F1A48;
extern u8 D_800C894C;                   /* bottom of the stack */

/* The search's working variables are globals, to keep the recursion's
 * frames small. */
extern Region *D_800F19E0;              /* neighbour */
extern Candidate D_800F19F0[3];         /* candidates */
extern Candidate *D_800F1A10[3];        /* candidates sorted by distance */
extern Candidate *D_800F1A1C;           /* swap */
extern s32 D_800F1A20;                  /* candidate count */
extern s32 D_800F1A24;                  /* edge */
extern s32 D_800F1A28;                  /* node count */
extern s32 D_800F1A2C;                  /* neighbour's edge back */
extern CVector3 D_800F1A30;             /* neighbour's centre */
extern f32 D_800F1A3C;                  /* cost */
extern s32 D_800F1A40;                  /* found */
extern s32 D_800F1A44;                  /* passable */

f32 func_0026B858(Region *from, Region *to);
void func_0026CB2C(Region *region, CVector3 *centre);
s32 func_0023B664(void *mover, Region *region, s32 edge, CVector3 *centre);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);

#define IN_BOUNDS(p) ((p)->x >= D_800F1A48->bounds[0] && (p)->x <= D_800F1A48->bounds[2] \
                      && (p)->z >= D_800F1A48->bounds[1] && (p)->z <= D_800F1A48->bounds[3])

/* Walks the region graph from region, depth first and nearest neighbour
 * first, appending a node for each step and keeping in path the cheapest
 * node that reached the start region. */
void func_002393D0(PathNode *parent, Region *region) {
    s8 next[3];
    PathNode *node;

    if ((u32) &parent < (u32) &D_800C894C) {
        return;
    }
    if (D_800F1A48->nodeCount >= 150) {
        return;
    }
    if (!(IN_BOUNDS(region->corner[0]) || IN_BOUNDS(region->corner[1]) || IN_BOUNDS(region->corner[2]))) {
        return;
    }
    if (parent != NULL) {
        D_800F1A3C = parent->cost + func_0026B858(parent->region, region);
        D_800F1A28 = parent->count + 1;
        if (D_800F1A48->path != NULL && D_800F1A48->path->cost <= D_800F1A3C) {
            return;
        }
    } else {
        D_800F1A3C = 0.0f;
        D_800F1A28 = 0;
    }
    node = D_800F1A48->poolNext;
    D_800F1A48->poolNext = node + 1;
    D_800F1A48->nodeCount++;
    node->region = region;
    node->parent = parent;
    node->count = D_800F1A28;
    node->cost = D_800F1A3C;
    if (region == D_800F1A48->startRegion) {
        if (D_800F1A48->path == NULL || D_800F1A3C < D_800F1A48->path->cost) {
            D_800F1A48->path = node;
        }
        return;
    }
    D_800F1A20 = 0;
    D_800F1A24 = 0;
    region->flags |= 0x8000;
    do {
        D_800F19E0 = region->neighbour[D_800F1A24];
        if (D_800F19E0 != NULL && !(D_800F19E0->flags & 0x8000)) {
            D_800F1A40 = 0;
            for (D_800F1A2C = 0; D_800F1A2C < 3; D_800F1A2C++) {
                if (D_800F19E0->neighbour[D_800F1A2C] == region) {
                    D_800F1A40 = 1;
                    break;
                }
            }
            if (D_800F1A40) {
                func_0026CB2C(D_800F19E0, &D_800F1A30);
                D_800F1A44 = func_0023B664(D_800F1A48->mover, D_800F19E0, D_800F1A2C, &D_800F1A30);
                if (D_800F1A44) {
                    func_0020EF2C(&D_800F1A30, &D_800F1A30, &D_800F1A48->startPos);
                    D_800F1A3C = D_800F1A30.x * D_800F1A30.x + D_800F1A30.y * D_800F1A30.y
                               + D_800F1A30.z * D_800F1A30.z;
                    D_800F19F0[D_800F1A20].edge = D_800F1A24;
                    D_800F19F0[D_800F1A20++].dist = D_800F1A3C;
                }
            }
        }
        D_800F1A24++;
    } while (D_800F1A24 < 3);

    switch (D_800F1A20) {
        case 0:
            return;
        case 1:
            next[1] = -1;
            next[2] = -1;
            next[0] = D_800F19F0[0].edge;
            break;
        case 2:
            if (D_800F19F0[0].dist < D_800F19F0[1].dist) {
                next[0] = D_800F19F0[0].edge;
                next[1] = D_800F19F0[1].edge;
            } else {
                next[0] = D_800F19F0[1].edge;
                next[1] = D_800F19F0[0].edge;
            }
            next[2] = -1;
            break;
        case 3:
            D_800F1A10[0] = &D_800F19F0[0];
            D_800F1A10[1] = &D_800F19F0[1];
            D_800F1A10[2] = &D_800F19F0[2];
            if (D_800F1A10[0]->dist > D_800F1A10[1]->dist) {
                D_800F1A1C = D_800F1A10[0];
                D_800F1A10[0] = D_800F1A10[1];
                D_800F1A10[1] = D_800F1A1C;
            }
            if (D_800F1A10[1]->dist > D_800F1A10[2]->dist) {
                D_800F1A1C = D_800F1A10[1];
                D_800F1A10[1] = D_800F1A10[2];
                D_800F1A10[2] = D_800F1A1C;
            }
            if (D_800F1A10[0]->dist > D_800F1A10[1]->dist) {
                D_800F1A1C = D_800F1A10[0];
                D_800F1A10[0] = D_800F1A10[1];
                D_800F1A10[1] = D_800F1A1C;
            }
            next[0] = D_800F1A10[0]->edge;
            next[1] = D_800F1A10[1]->edge;
            next[2] = D_800F1A10[2]->edge;
            break;
    }
    if (next[0] != -1) {
        func_002393D0(node, region->neighbour[next[0]]);
    }
    if (next[1] != -1) {
        func_002393D0(node, region->neighbour[next[1]]);
    }
    if (next[2] != -1) {
        func_002393D0(node, region->neighbour[next[2]]);
    }
}
