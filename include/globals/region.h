#ifndef GLOBALS_REGION_H
#define GLOBALS_REGION_H

#include "common.h"
#include "structs/vector.h"

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

/* CGameRegion normal caches: the region's raw and unit normals, the ceiling's (y and z have symbols of their own) */
extern CVector3 sRegionNormal;
extern f32 sRegionNormal_y;           /* sRegionNormal.y */
extern f32 sRegionNormal_z;           /* sRegionNormal.z */
extern CVector3 sRegionUnitNormal;
extern f32 sRegionUnitNormal_y;       /* sRegionUnitNormal.y */
extern f32 sRegionUnitNormal_z;       /* sRegionUnitNormal.z */
extern CVector3 sRegionCeilingNormal;
extern CVector3 sRegionCeilingUnitNormal;

#endif
