#ifndef STRUCTS_PATH_H
#define STRUCTS_PATH_H

/* The region path search (Path_FindRegionPath, Path_ExpandNode): a search
 * through the region graph from the goal back to the object's region. */

#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/collision.h"

/* One step of a path, a node in the search's pool. Once the path is found,
 * parent is the next step towards the goal. */
typedef struct PathNode {
    /* 0x00 */ CGameRegion *region;
    /* 0x04 */ struct PathNode *parent;
    /* 0x08 */ s32 count;               /* steps from the goal */
    /* 0x0C */ f32 cost;
} PathNode; /* size 0x10 */

/* The path search state, kept on Path_FindRegionPath's stack and reached
 * through sPathSearch. */
typedef struct PathSearch {
    /* 0x000 */ CGameObjectInstance *obj;
    /* 0x004 */ CGameRegion *startRegion;
    /* 0x008 */ CVector3 startPos;
    /* 0x014 */ CGameRegion *goalRegion;
    /* 0x018 */ CVector3 goalPos;
    /* 0x024 */ f32 bounds[4];          /* min x, min z, max x, max z */
    /* 0x034 */ PathNode pool[150];
    /* 0x994 */ PathNode *poolNext;
    /* 0x998 */ s32 nodeCount;
    /* 0x99C */ PathNode *path;
    /* 0x9A0 */ Sweep mover;            /* who moves, for Sweep_CrossRegionEdge */
} PathSearch; /* size 0xA48 */

/* A neighbour worth exploring: its edge index and squared distance. */
typedef struct PathCandidate {
    /* 0x0 */ s32 edge;
    /* 0x4 */ f32 dist;
} PathCandidate;

#endif
