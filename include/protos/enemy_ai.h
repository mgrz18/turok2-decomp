#ifndef PROTOS_ENEMY_AI_H
#define PROTOS_ENEMY_AI_H

/* code/EnemyAI (src/us/code/EnemyAI/EnemyAI.c), the enemies' shared modes, animation pickers,
 * drawing and damage: the functions one part of it calls in another, and the calls it makes into
 * other modules, one prototype each on the real types (round 29). The AI functions come from
 * protos/cai.h (an EnemyAI file includes neither protos/ai.h nor protos/enemy.h, whose untyped
 * views the other enemy modules still call). */

#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/model.h"
#include "structs/collision.h"
#include "protos/game_object.h"
#include "protos/cai.h"

/* code/EnemyAI */
void AI_HearNoise(CGameObjectInstance *actor, CAI *ai, CVector3 *pos, s32 kind);
void EnemyAI_CalcOrientationMatrix(CGameObjectInstance *actor, CAI *ai);
void EnemyAI_DrawFaded(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params);
void EnemyAI_NukedFrozenRemove(CGameObjectInstance *obj, CAI *ai);
void EnemyMode_Move_Update(CGameObjectInstance *actor, CAI *ai);
void EnemyAI_CheckOffscreen(CGameObjectInstance *obj, CAI *ai, s32 unused);
/* The definition returns s16; the ROM's callers take the whole register (an s16 would add a sign
 * extension), the host's the s16 (it leaves the upper bits of the return register undefined). */
s16 EnemyAI_PickAttack(CGameObjectInstance *actor, CAI *ai, CAITargets *t);
#ifdef TARGET_PORT
#define EnemyAI_PickAttack_w EnemyAI_PickAttack
#else
#define EnemyAI_PickAttack_w ((s32 (*)(CGameObjectInstance *, CAI *, CAITargets *)) EnemyAI_PickAttack)
#endif

/* code/AI (their definitions still take views: these are the types the callers pass) */
void AI_UpdateTargets(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
s32 AI_IsAimedAt(CGameObjectInstance *self, CAI *ai, CGameObjectInstance *target, f32 cone);
void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 angle, f32 limit);

/* CPathTrack, CInstancePool, CGameObjectInstance, Throwable, EnemyAnim */
void CPathTrack__Advance(CPathTrack *track);
void CListedInstance__Free(void *obj);
void CGameObjectInstance__DrawModelImmediate(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params);
void Throwable_SetModeAnim(void *holder, CAI *ai, void *obj);
s32 EnemyAnim_Idle_AF6F0(CGameObjectInstance *actor, CAI *ai);

/* CMtxF and the vector math */
void CMtxF__RotateX(void *mf, f32 angle);
void CMtxF__RotateZ(void *mf, f32 angle);
f32 CVector3__Distance(CVector3 *a, CVector3 *b);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
f32 Math_Approach(f32 value, f32 target, f32 rate);

/* GameObject (protos/game.h's prototype: that header's GameObject_GetRegionFloorHeight is a view) */
void GameObject_FireTriggers(struct CGameObjectInstance *obj, s32 flags, sptr source);

/* Render, Sound */
void Render_DrawModelShared(void **resource, void *matrix, s32 segment, void *lights, void *textures, s32 lod, s32 pass);
s32 Sound__Play(s16 sound, CVector3 pos, sptr follow, sptr owner);

#endif
