#include "common.h"
#include "structs/game.h"

extern s32 D_8011ACF4;
extern s32 D_800B6D20;
extern f32 gFrameIncrement[2];   /* frame time scales */
extern f32 D_800B6D30;
extern f32 D_800B6D34;
extern u8 gScene[];
extern s32 D_801308F8;

extern void CEngineApp__UpdateControllers(void);
extern void Demo_PlayNext(void);
extern s32 SavedPlayers_IsActive(void);
extern s32 SavedPlayers_GetWarpID(void);
extern void func_002888BC(CEngineApp *, s32, s32);
extern void SavedPlayers_Restore(void);
extern void CCollisionInfo__Reset(void);
extern void func_00430500(void *);
extern void CScene__InitPersistentData(void *, s32);
extern void CScene__RequestWarp(void *, s32, s32);

void EngineMode13_Game_Init(CEngineApp *state) {
    s32 flag;

    D_8011ACF4 = 0;
    D_800B6D20 = 0;
    CEngineApp__UpdateControllers();
    flag = 1;
    Demo_PlayNext();
    if (state->m_Settings.prevMode == 11 || state->m_Settings.prevMode == 12 || state->m_Settings.warpFlags != 0) {
        flag = 0;
        if (state->m_Settings.warpFlags != 0) {
            state->m_Settings.warpFlags = 0;
        } else {
            state->m_Settings.matchMode = 1000;
        }
    }
    if (SavedPlayers_IsActive()) {
        flag = 0;
        state->m_Settings.matchMode = SavedPlayers_GetWarpID();
    }
    func_002888BC(state, state->m_Settings.playerCount, flag);
    if (SavedPlayers_IsActive()) {
        SavedPlayers_Restore();
    }
    CCollisionInfo__Reset();
    D_800B6D34 = 1.0f;
    gFrameIncrement[1] = 1.0f;
    D_800B6D30 = 1.0f;
    func_00430500(&state->m_Game.m_DebugLines);
    CScene__InitPersistentData(gScene, state->m_CartridgeIndex);
    CScene__RequestWarp(gScene, state->m_Settings.matchMode, 0);
    D_801308F8 = 0;
}
