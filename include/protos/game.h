/* other functions with many disagreeing declarations.
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_GAME_H
#define PROTOS_GAME_H

#include "common.h"

struct CGameObjectInstance;
struct ControllerProfile;
struct LoopSounds;

void Controller_SetRumbleOption(struct ControllerProfile *, s32);
void GameObject_FireTriggers(struct CGameObjectInstance *, s32, s32);
f32 GameObject_GetRegionFloorHeight(void *, void *);
struct LoopSounds *LoopSounds_Get(void);
void Render_DrawModel(void **, void *, s32, char *, void *, s32, s32);

#endif
