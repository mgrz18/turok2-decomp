#ifndef GLOBALS_SCENE_H
#define GLOBALS_SCENE_H

/* The level (gScene, Turok 3's CScene) and the symbols the code keeps for
 * fields inside it (gScene_m_<field> is &gScene.<field>). */

#include "structs/world.h"
#include "structs/intelligence.h"

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

#endif
