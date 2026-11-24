#ifndef PROTOS_CAI_H
#define PROTOS_CAI_H

/* code/AI (src/us/code/AI/AI.c), the engine's AI step every enemy, platform and pickup runs, on
 * its real types: one prototype each. protos/ai.h and protos/enemy.h keep the untyped views the
 * enemy modules call these through. */

#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"

void AI_InitHealth(CAI *ai, CIntelligence *def, CVector3 pos, CGameObjectInstance *obj);
void AI_RideMovingPlatform(CGameObjectInstance *actor, CAI *ai);
void AI_UpdateRegionFlags(CGameObjectInstance *actor, CAI *ai);
void CAI__UpdateModeAnim(CGameObjectInstance *obj, CAI *ai);
s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
s32 AI_IsInViewCone(CGameObjectInstance *viewer, CGameObjectInstance *target, f32 range, f32 across, f32 up);
s32 AI_UpdatePathHeading(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target);
CGameObjectInstance *AI_FindBestTarget(CGameObjectInstance *self, CAI *plan, s32 checkSight, s32 flag);
void AI_GetPathTargetInfo(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target, CAITarget *out);
void AI_GetSightTargetInfo(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target, CAITarget *out);
f32 AI_FindClearHeading(CGameObjectInstance *self, CVector3 pos, CGameObjectInstance *target, f32 scale);
void AI_ComputeFrameMotion(CGameObjectInstance *obj, CAI *ai, CAIFrameMotion *out);
f32 AI_AngleToPoint(CGameObjectInstance *actor, CVector3 point);
void Actor_TurnBy(CGameObjectInstance *actor, f32 angle);
void AI_StopSound(CGameObjectInstance *obj, CAI *ai);
f32 AI_DistSqToPointAtHeight(CGameObjectInstance *obj, CVector3 point);
f32 AI_FlatDistSqToPoint(CGameObjectInstance *obj, CVector3 point);

#endif
