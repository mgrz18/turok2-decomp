/* AI helpers the enemy modules call (AI, EnemyAI): one prototype for each, written by
 * hand from the definitions. The actor, AI and output records are void * so the
 * callers keep their own views (CGameObjectInstance / CPlayer, CAI / its plan,
 * CAITargets / a module record); none of these changes the N64 code. */

#ifndef PROTOS_ENEMY_H
#define PROTOS_ENEMY_H

#include "common.h"

void AI_UpdateTargets(void *self, void *ai, void *out, s32 f1, s32 f2);
void AI_ComputeFrameMotion(void *self, void *ai, void *out);
f32 AI_LerpByAggression(void *self, void *ai, f32 lo, f32 hi);
s32 AI_IsAimedAt(void *self, void *ai, void *target, f32 angle);
#ifdef TARGET_PORT
s8 EnemyAI_GetBehaviorIndex(void *self, void *ai);  /* host: the definition's s8 (the upper bits of the return register are undefined) */
#else
s32 EnemyAI_GetBehaviorIndex(void *self, void *ai); /* N64: the callers take the whole register, as the ROM does (s8 would add a sign extension) */
#endif

#endif
