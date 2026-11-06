#ifndef GLOBALS_SCENE_H
#define GLOBALS_SCENE_H

/* Scene globals outside CScene: lighting switches and keys, the level's
 * display list, the warp fade table, the dynamic light buffers. */

#include "common.h"
#include "gfx.h"
#include "structs/warp_fade.h"

/* Actors_GetLightVolumeFalloff, by squared distance: shape 0 (sphere), shape 2 (cone). */
extern f32 sLightSphereFalloff[65];
extern f32 sLightConeFalloff[65];

extern s32 gLitVerticesKey;         /* GameSettings +0xC | 0x100/0x200 (cheats): ORed into the lit vertices' cache keys */
extern s32 gLitVerticesKeyChanged;  /* set by CScene__Update when gLitVerticesKey changed */
extern s32 gLevelLightsEnabled;     /* 1; 0 skips the level lights (debug switch) */
extern s32 gNextUniqueId;           /* Instance_NextUniqueId, from 0x380000 */
extern s8 gColorPulseTable[64];     /* the colour pulse (Model_PulseVertexColors, LightVolume_GetFlicker) */

extern Gfx *gSceneGeometryDL;       /* the level's display list once built this frame */
extern s32 gCinemaHidesActors;      /* 1: a playing cinema path draws only actors flagged 0x200 */
extern CWarpFadeColor gWarpFadeColors[3];   /* by CScene m_WarpFade */
extern s32 gDLightBufferOffset;     /* 0 / 0x20, flipped by DLights_SwapBuffers */

#endif
