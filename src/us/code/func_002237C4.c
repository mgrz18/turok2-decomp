/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/CODE_80289790.CPP, Scene_UpdateSelectionTransition, with Turok 2's layouts. */

#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "globals/engine.h"

typedef struct ColorEntry {
    /* 0x00 */ f32 value;               /* the fade's length */
    /* 0x04 */ u8 pad4[4];
} ColorEntry;

extern CGameFromCameraPool gEngineApp_m_Game_m_CameraPool; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */
extern ColorEntry gWarpFadeColors[];
extern u8 D_800B54E0[];

void Cinema_Reset(ColorEntry *entry);
void Cinema_StopAtEnd(void);
void Cinema_RequestById(s32 arg0);
s32 CScene__FindWarpDests(CScene *world, s32 arg1, s32 id, CWarpPoint *spawn, s32 arg4);
void CScene__LoadLevel(CScene *world, s32 id, s32 flags);
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
    ColorEntry *entry = &gWarpFadeColors[scene->m_WarpFade];
    s32 id;
    GameSettings *settings;

    switch (scene->m_WarpState) {
        case 0:
            break;
        case 1:
            if (scene->m_WarpTime == entry->value) {
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
                    settings = &gEngineApp_m_Game_m_CameraPool.m_Settings;
                    if (scene->m_PreviewOptions != settings->unlockedCheats && !Demo_GetPlayerCount()) {
                        CCameraPool__AddTimedText(&gEngineApp_m_Game_m_CameraPool, gEngineApp_m_Game_m_CameraPool.m_CameraPool + 0x40, D_800B54E0, 4.0f);
                        scene->m_PreviewOptions = settings->unlockedCheats;
                    }
                }
            } else {
                scene->m_WarpTime += gFrameIncrement;
                if (entry->value < scene->m_WarpTime) {
                    scene->m_WarpTime = entry->value;
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
