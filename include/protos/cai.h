#ifndef PROTOS_CAI_H
#define PROTOS_CAI_H

/* code/AI (src/us/code/AI/AI.c), the engine's AI step every enemy, platform and pickup runs, on
 * its real types: one prototype each. protos/ai.h and protos/enemy.h keep the untyped views the
 * enemy modules call these through. */

#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/enemy_anim.h"

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
s32 AI_PickAnimFromTable(CGameObjectInstance *owner, sptr unused, CEnemyAnimPick *table, s32 flags);
void AI_TurnTowardTarget(CGameObjectInstance *o, CAI *ai, CGameObjectInstance *t, f32 lim);
void AI_DropPickupsOnce(CInstanceHeader *o, u32 *flags, u32 bit, u32 m1, u32 m2);
struct CDropPickup;
void AI_DropPickupsByMask(CInstanceHeader *o, s32 a1, u32 mask, struct CDropPickup *e);
void Actor_SpawnParticle(CInstanceHeader *obj, s32 kind, CVector3 at, s32 unused);
void AI_StartSound(CInstanceHeader *src, CAI *owner, s32 id);
void AI_PushAway(CGameObjectInstance *a, s32 unused, CGameObjectInstance *b, CGameObjectInstance *c,
                 CVector3 *outPos, CGameRegion **outRegion);

/* The calls code/AI makes into other modules (round 29), one prototype each: the types are the
 * callee definitions' (or, where those are still untyped, protos/game.h's). */
CPathPoint *CPathTrack__GetCurrentPoint(CPathTrack *track);
sptr CGameObjectInstance__PickRandomAnim(CGameObjectInstance *actor, sptr action, sptr skip);
CQuaternion CGameObjectInstance__GetRotation(CGameObjectInstance *actor);
s32 CGameObjectInstance__GetGeometryKind(void *obj);
CQuaternion Actor_GetGroundRotation(void *obj);
void CQuaternion__ToMatrix(CQuaternion *q, void *mf); /* a CMtxF */
s32 CParticleSystem__Spawn(void *, void *, void *, s32 *, sptr, s32, CVector3, CQuaternion, CVector3, void *, s32, s32);
struct LoopSounds *LoopSounds_Get(void);
void *LoopSounds_Start(void *sounds, s32 id, CVector3 *pos, void *follow, sptr owner);
s32 LoopSounds_Stop(void *sounds, void *source);
void Sound__StopOwner(sptr owner);
f32 Random_Blend(f32 a, f32 b);

#endif
