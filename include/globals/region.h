#ifndef GLOBALS_REGION_H
#define GLOBALS_REGION_H

#include "common.h"

struct CGameRegion;
struct CRegionInfo;

/* GameObject_ApplyCurrent: {blend rate, turn rate} of a material current */
extern f32 sSurfaceCurrentRates[2];

/* CGameRegion one-entry caches: the region whose result is in the cache buffers */
extern struct CGameRegion *sCachedCeilingRawNormalRegion;
extern struct CGameRegion *sCachedCeilingNormalRegion;
extern struct CGameRegion *sCachedRawNormalRegion;
extern struct CGameRegion *sCachedNormalRegion;
extern struct CGameRegion *sCachedRegionInfoRegion;
extern struct CRegionInfo *sCachedRegionInfo;

#endif
