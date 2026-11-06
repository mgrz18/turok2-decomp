#ifndef GLOBALS_PATH_H
#define GLOBALS_PATH_H

/* The region path search (structs/path.h). Path_ExpandNode recurses, so its
 * working variables are globals, to keep its frames small. */

#include "structs/path.h"

extern PathSearch *sPathSearch;             /* the search running */
extern CGameRegion *sPathNeighbour;         /* the neighbour looked at */
extern PathCandidate sPathCandidates[3];    /* the neighbours worth exploring */
extern PathCandidate *sPathSorted[3];       /* ... sorted by distance */
extern PathCandidate *sPathSwap;
extern s32 sPathCandidateCount;
extern s32 sPathEdge;                       /* the edge crossed */
extern s32 sPathDepth;                      /* nodes in the path */
extern s32 sPathBackEdge;                   /* the neighbour's edge back */
extern CVector3 sPathCentre;                /* the neighbour's centre */
extern f32 sPathCost;
extern s32 sPathFound;
extern s32 sPathCrossed;                    /* the edge is passable */

#endif
