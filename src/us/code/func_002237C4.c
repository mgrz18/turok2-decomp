/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/CODE_80289790.CPP, Scene_UpdateSelectionTransition, with Turok 2's layouts. */

#include "common.h"
#include "structs/world.h"
#include "structs/game.h"

typedef struct ColorEntry {
    /* 0x00 */ f32 value;               /* the fade's length */
    /* 0x04 */ u8 pad4[4];
} ColorEntry;

extern CGameFromCameraPool D_801198B0;
extern ColorEntry D_800ACDE8[];
extern f32 D_800B6D28;                  /* the frame's time step */
extern u8 D_800B54E0[];

void func_0028441C(ColorEntry *entry);
void func_00412B14(void);
void func_00412438(s32 arg0);
s32 func_0042C194(CWorld *world, s32 arg1, s32 id, CWarpPoint *spawn, s32 arg4);
void func_0042AC0C(CWorld *world, s32 id, s32 flags);
s32 func_0027BC34(void);
void func_00281610(CGameFromCameraPool *game, void *camera, u8 *text, f32 time);

static __inline__ s32 transitionId(CWorld *scene) {
    if (scene->m_bWarpSpawn != 0) {
        return scene->m_WarpSpawn.m_nLevel;
    }
    return scene->m_WarpID;
}

/* Runs the fade of a level transition: state 1 fades in to the entry length and then warps
 * (func_0042C194, or straight to ~id for a negative id), state 2 fades back out. */
void func_002237C4(CWorld *scene) {
    ColorEntry *entry = &D_800ACDE8[scene->m_WarpFade];
    s32 id;
    GameSettings *settings;

    switch (scene->m_WarpState) {
        case 0:
            break;
        case 1:
            if (scene->m_WarpTime == entry->value) {
                func_0028441C(entry);
                if (scene->m_WarpID == 999) {
                    func_00412B14();
                    func_00412438(400);
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
                    scene->m_bWarpSpawn = func_0042C194(scene, -1, scene->m_WarpID, &scene->m_WarpSpawn, 1);
                }
                scene->m_LevelID = scene->m_WarpID;
                id = transitionId(scene);
                func_0042AC0C(scene, id, scene->m_bWarpSpawn);
                if (scene->m_bWarpSpawn != 0) {
                    settings = &D_801198B0.m_Settings;
                    if (scene->m_PreviewOptions != settings->unlockedCheats && !func_0027BC34()) {
                        func_00281610(&D_801198B0, D_801198B0.m_CameraPool + 0x40, D_800B54E0, 4.0f);
                        scene->m_PreviewOptions = settings->unlockedCheats;
                    }
                }
            } else {
                scene->m_WarpTime += D_800B6D28;
                if (entry->value < scene->m_WarpTime) {
                    scene->m_WarpTime = entry->value;
                }
            }
            break;
        case 2:
            if (scene->m_WarpTime == 0.0f) {
                scene->m_WarpState = 0;
            } else {
                scene->m_WarpTime -= D_800B6D28;
                if (scene->m_WarpTime < 0.0f) {
                    scene->m_WarpTime = 0.0f;
                }
            }
            break;
    }
}
