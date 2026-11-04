#include "common.h"
#include "structs/game.h"

extern s32 D_8011ACF4;
extern s32 D_800B6D20;
extern f32 D_800B6D28[2];   /* frame time scales */
extern f32 D_800B6D30;
extern f32 D_800B6D34;
extern u8 D_800F7078[];
extern s32 D_801308F8;

extern void func_002886AC(void);
extern void func_0042F590(void);
extern s32 func_0028D0E0(void);
extern s32 func_0028D0D0(void);
extern void func_002888BC(CEngineApp *, s32, s32);
extern void func_0028D100(void);
extern void CCollisionInfo__Reset(void);
extern void func_00430500(void *);
extern void func_0042C3BC(void *, s32);
extern void func_0042C700(void *, s32, s32);

void func_00289910(CEngineApp *state) {
    s32 flag;

    D_8011ACF4 = 0;
    D_800B6D20 = 0;
    func_002886AC();
    flag = 1;
    func_0042F590();
    if (state->m_Settings.prevMode == 11 || state->m_Settings.prevMode == 12 || state->m_Settings.warpFlags != 0) {
        flag = 0;
        if (state->m_Settings.warpFlags != 0) {
            state->m_Settings.warpFlags = 0;
        } else {
            state->m_Settings.matchMode = 1000;
        }
    }
    if (func_0028D0E0()) {
        flag = 0;
        state->m_Settings.matchMode = func_0028D0D0();
    }
    func_002888BC(state, state->m_Settings.playerCount, flag);
    if (func_0028D0E0()) {
        func_0028D100();
    }
    CCollisionInfo__Reset();
    D_800B6D34 = 1.0f;
    D_800B6D28[1] = 1.0f;
    D_800B6D30 = 1.0f;
    func_00430500(&state->m_Game.m_DebugLines);
    func_0042C3BC(D_800F7078, state->m_CartridgeIndex);
    func_0042C700(D_800F7078, state->m_Settings.matchMode, 0);
    D_801308F8 = 0;
}
