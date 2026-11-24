#ifndef PROTOS_IMPACT_EVENT_H
#define PROTOS_IMPACT_EVENT_H

/* The calls code/ImpactEvent makes into other modules, one prototype each. */

#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"

f32 GameObject_GetCollisionHeight(CGameObjectInstance *a);
f32 CGameObjectInstance__GetAverageScale(CGameObjectInstance *arg0);
void GameObject_TraceMove(CGameObjectInstance *actor, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 flagA, s32 flagB);

#endif
