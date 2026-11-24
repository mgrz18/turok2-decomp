#ifndef PROTOS_GAME_OBJECT_H
#define PROTOS_GAME_OBJECT_H

/* code/GameObject (src/us/code/GameObject/GameObject.c): an object's collision sizes, floors,
 * regions and definition flags; and the calls it makes into other modules, one prototype each. */

#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"
#include "structs/game_object.h"
#include "structs/collision.h"

/* code/GameObject */
f32 GameObject_GetCollisionHeight(CGameObjectInstance *a);
f32 GameObject_GetDeadCollisionHeight(CGameObjectInstance *actor);
f32 GameObject_GetGroundHeight(CGameObjectInstance *actor);
void GameObject_GetFrameDelta(CGameObjectInstance *obj, CVector3 *delta);
f32 GameObject_GetFloorHeightAt(CGameObjectInstance *actor, f32 x, f32 z);
f32 GameObject_GetRegionFloorHeight(CInstanceHeader *obj, CVector3 *offset);
void GameObject_TraceMove(CInstanceHeader *actor, CVector3 pos, CVector3 *outPos, CGameRegion **outRegion, s32 flagA, s32 flagB);

/* CGameRegion, PlatformCollide */
f32 CGameRegion__GetHeight(CGameRegion *tri, f32 x, f32 z);
f32 CGameRegion__GetClimbAngle(CGameRegion *tri, f32 x1, f32 z1, f32 x2, f32 z2);
void CGameRegion__GetNormal(CVector3 *out, CGameRegion *tri); /* returns out, which no caller here reads */
s32 CGameRegion__IsSteep(CGameRegion *polygon);
f32 Platform_GetTopHeight(void *object, f32 x, f32 z);

/* SweepShapes */
s32 Collision_MoveInstance(CGameObjectInstance *obj, CVector3 pos, CCollisionInfo *def);

/* CMtxF and the gu math */
void CVector3__Normalize(CVector3 *v);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
CVector3 CVector3__RandomPerturb(CVector3 *in, f32 amount);
void CQuaternion__SlerpFast(CQuaternion *out, f32 t, CQuaternion *a, CQuaternion *b);
f32 Math_AcosLookup(f32 x);
f32 func_002974C0(f32 x); /* cosf */
f32 func_002982F0(f32 x); /* sinf */

#endif
