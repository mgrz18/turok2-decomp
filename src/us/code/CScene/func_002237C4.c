/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/CODE_80289790.CPP, Scene_UpdateSelectionTransition, with Turok 2's layouts. */

#include "common.h"
#include "globals/strings.h"
#include "structs/world.h"
#include "structs/game.h"
#include "globals/engine.h"
#include "globals/scene.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
s32 CScene__FindWarpDests(void *, s32, s32, void *, s32);
void CScene__LoadLevel(struct CScene *, s32, s32);
void CScene__UpdateWarp(struct CScene *);

extern CGameFromCameraPool gCameraPoolGame_002237C4 __asm__("gEngineApp_m_Game_m_CameraPool"); /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */

void Cinema_Reset(CWarpFadeColor *entry);
void Cinema_StopAtEnd(void);
void Cinema_RequestById(s32 arg0);
s32 Demo_GetPlayerCount(void);
void CCameraPool__AddTimedText(CGameFromCameraPool *game, void *camera, u8 *text, f32 time);

static __inline__ s32 transitionId(CScene *scene) {
    if (scene->m_bWarpSpawn != 0) {
        return scene->m_WarpSpawn.m_nLevel;
    }
    return scene->m_WarpID;
}

/* Runs the fade of a level transition: state 1 fades in to the entry length and then warps
 * (CScene__FindWarpDests, or straight to ~id for a negative id), state 2 fades back out. */
void CScene__UpdateWarp(CScene *scene) {
    CWarpFadeColor *entry = &gWarpFadeColors[scene->m_WarpFade];
    s32 id;
    GameSettings *settings;

    switch (scene->m_WarpState) {
        case 0:
            break;
        case 1:
            if (scene->m_WarpTime == entry->length) {
                Cinema_Reset(entry);
                if (scene->m_WarpID == 999) {
                    Cinema_StopAtEnd();
                    Cinema_RequestById(400);
                }
                if (scene->m_WarpFade == 0) {
                    scene->m_WarpState = 0;
                } else {
                    scene->m_WarpState = 2;
                }
                if (scene->m_WarpID < 0) {
                    scene->m_WarpID = ~scene->m_WarpID;
                    scene->m_bWarpSpawn = 0;
                } else {
                    scene->m_bWarpSpawn = CScene__FindWarpDests(scene, -1, scene->m_WarpID, &scene->m_WarpSpawn, 1);
                }
                scene->m_LevelID = scene->m_WarpID;
                id = transitionId(scene);
                CScene__LoadLevel(scene, id, scene->m_bWarpSpawn);
                if (scene->m_bWarpSpawn != 0) {
                    settings = &gCameraPoolGame_002237C4.m_Settings;
                    if (scene->m_PreviewOptions != settings->unlockedCheats && !Demo_GetPlayerCount()) {
#ifdef TARGET_PORT
                        CCameraPool__AddTimedText(&gCameraPoolGame_002237C4, &gCameraPoolGame_002237C4.m_CameraPool[0].m_MainCamera, sCheatMessageText_NewCheatEnabled, 4.0f);
#else
                        CCameraPool__AddTimedText(&gCameraPoolGame_002237C4, gCameraPoolGame_002237C4.m_CameraPool + 0x40, sCheatMessageText_NewCheatEnabled, 4.0f);
#endif
                        scene->m_PreviewOptions = settings->unlockedCheats;
                    }
                }
            } else {
                scene->m_WarpTime += gFrameIncrement;
                if (entry->length < scene->m_WarpTime) {
                    scene->m_WarpTime = entry->length;
                }
            }
            break;
        case 2:
            if (scene->m_WarpTime == 0.0f) {
                scene->m_WarpState = 0;
            } else {
                scene->m_WarpTime -= gFrameIncrement;
                if (scene->m_WarpTime < 0.0f) {
                    scene->m_WarpTime = 0.0f;
                }
            }
            break;
    }
}
