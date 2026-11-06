#ifndef GLOBALS_SCENE_H
#define GLOBALS_SCENE_H

/* Fields of the world, gScene (CScene, structs/world.h), with symbols of their
 * own: the files reach them by symbol, as the ROM bases the accesses there. */

#include "structs/world.h"
#include "structs/intelligence.h"
#include "common.h"
#include "gfx.h"
#include "structs/warp_fade.h"

extern f32 gScene_m_WarpTime;            /* gScene.m_WarpTime */
extern s32 gScene_m_bWarpSpawn;          /* gScene.m_bWarpSpawn */
extern CWarpPoint gScene_m_WarpSpawn;    /* gScene.m_WarpSpawn */
extern s32 gScene_m_LevelID;             /* gScene.m_LevelID: the warp id of the loaded level */
extern s32 gScene_m_WarpID;              /* gScene.m_WarpID */
extern s32 gScene_m_PendingWarpID;       /* gScene.m_PendingWarpID */
extern s32 gScene_m_PreviewState;        /* gScene.m_PreviewState */
extern s32 gScene_m_PreviewTimer;        /* gScene.m_PreviewTimer */
extern s32 gScene_m_PreviewOptions;      /* gScene.m_PreviewOptions */
extern CGeomHeader gScene_m_Geom[5];     /* gScene.m_Geom */

/* The level (gScene, Turok 3's CScene) and the symbols the code keeps for
 * fields inside it (gScene_m_<field> is &gScene.<field>). */

extern CScene gScene;

/* Field symbols, kept where a file reaches the field by its own symbol
 * (a %hi/%lo pair or a base register GCC does not share with gScene). */
extern s32 gScene_m_CinemasKey;
extern s32 *gScene_m_pCinemas;
extern s32 gScene_m_nDrawActors;
extern CList gScene_m_InstancePool_m_FreeList;
extern u8 *gScene_m_pSections;
extern s32 gScene_m_nSections;
extern s32 gScene_m_nLevel;
extern s32 gScene_m_WarpFade;
extern s32 gScene_m_WarpState;

/* The bit sections of the level: m_pBitSections (saved with the game), m_pBitSections2. */
extern u8 gLoadSavePersistentData[0x5800];
extern u8 gGamePersistentData[0x5000];

/* The definitions the level actor pool and the particle system give their objects. */
extern CIntelligence gLevelActorIntelligenceMP;  /* level actors in multiplayer */
extern CIntelligence gLevelActorIntelligence;    /* level actors in single player */
extern CIntelligence gParticleIntelligence;      /* every particle */

/* Scene globals outside CScene: lighting switches and keys, the level's
 * display list, the warp fade table, the dynamic light buffers. */

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
