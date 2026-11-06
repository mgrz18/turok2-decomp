#ifndef GLOBALS_SCENE_H
#define GLOBALS_SCENE_H

/* The level (gScene, Turok 3's CScene) and the symbols the code keeps for
 * fields inside it (gScene_m_<field> is &gScene.<field>). */

#include "structs/world.h"

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

#endif
