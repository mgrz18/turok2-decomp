#include "common.h"
#include "structs/game.h"
#include "globals/scene.h"

extern s32 gEngineApp_m_Settings_match_unk44;
extern s32 gFrameCountGameplay;
extern f32 gFrameIncrement[2];   /* frame time scales */
extern f32 gParticleTimeScale;
extern f32 gEmitterTimeScale;
extern s32 gInSinglePlayerMenu;

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

    gEngineApp_m_Settings_match_unk44 = 0;
    gFrameCountGameplay = 0;
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
    gEmitterTimeScale = 1.0f;
    gFrameIncrement[1] = 1.0f;
    gParticleTimeScale = 1.0f;
    func_00430500(&state->m_Game.m_Automap);
    CScene__InitPersistentData(&gScene, state->m_CartridgeIndex);
    CScene__RequestWarp(&gScene, state->m_Settings.matchMode, 0);
    gInSinglePlayerMenu = 0;
}
