#ifndef GLOBALS_SCENE_H
#define GLOBALS_SCENE_H

/* Fields of the world, gScene (CScene, structs/world.h), with symbols of their
 * own: the files reach them by symbol, as the ROM bases the accesses there. */

#include "structs/world.h"

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

#endif
