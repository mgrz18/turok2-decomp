/* CGameRegion, a floor triangle of the collision mesh (structs/region.h): the signatures of the
 * definitions in src/us/code/CGameRegion that the module's own files call (round 28, part q2). */

#ifndef PROTOS_REGION_H
#define PROTOS_REGION_H

#include "common.h"
#include "structs/vector.h"

struct CGameRegion;

CVector3 *CGameRegion__GetCeilingNormal(CVector3 *out, struct CGameRegion *node);
f32 CGameRegion__GetCeilingHeight(struct CGameRegion *node, f32 x, f32 z);
f32 CGameRegion__GetHeight(struct CGameRegion *tri, f32 x, f32 z);
s32 CGameRegion__IsSteep(struct CGameRegion *polygon);
void CGameRegion__ClearVisited(struct CGameRegion *region);
void CGameRegion__FloodSetFlag4(struct CGameRegion *region);
void CGameRegion__FloodClearFlag4(struct CGameRegion *region);
void CGameRegion__FloodSetFlag4Same(struct CGameRegion *region, u16 info);
void CGameRegion__FloodClearFlag4Same(struct CGameRegion *region, u16 info);
void CGameRegion__FloodSetFlag4000Same(struct CGameRegion *region, u16 info);
void CGameRegion__FloodSetWaterSame(struct CGameRegion *region, u16 info);
void CGameRegion__FloodClearWaterAll(struct CGameRegion *region);
void CGameRegion__FloodSetHeight(struct CGameRegion *region, f32 height, s32 info);

#endif
