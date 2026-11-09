/* AI helpers called from every enemy.
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_AI_H
#define PROTOS_AI_H

#include "common.h"

struct IntPair;

s32 AI_PickAnimFromTable(sptr, sptr, struct IntPair *, s32);
void AI_PlayAnim(sptr, void *, s32);
void AI_TurnClamped(void *, sptr, f32, f32);
void CAI__CallHurt(s32, void *);
s32 CAI__SetMode(void *, void *, s32);

#endif
