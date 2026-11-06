/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00292910.c, func_00292910, with Turok 2's layouts. */
#include "common.h"
#include "structs/game.h"
#include "structs/frame.h"

typedef struct VideoRate {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ f32 hz;
} VideoRate;

extern u64 D_8011AD08;          /* time of the last frame */
extern s32 D_8011AD10;          /* retraces this frame */
extern u32 D_8011B09C;          /* retrace counts of the last four frames */
extern u32 D_8011B10C;
extern u32 D_800F6CA8;
extern u32 D_8011AD14;
extern u32 D_801198A0;
extern s32 D_801308F8;
extern s32 D_800C2034;          /* screen height */
extern VideoRate D_8011B0A0;
extern f32 D_800B6D28;          /* frame time */
extern s32 D_801198A8;
extern s32 D_80130940;
extern u8 D_80119E2C[];
extern s32 D_8011ACF4;

extern u64 osGetTime(void);     /* osGetTime */
extern s32 MenuPool_CountOpen(void *);
extern s32 Demo_GetPlayerCount(void);
extern s32 Cinema_PathPlaying(void);
extern void Demo_AdvanceMatchStream(void);
extern void CCache__Advance(s32);
extern void CEngineApp__Draw(CEngineApp *);
extern void CEngineApp__UpdatePlayTime(CEngineApp *);
extern s32 Cinema_GetScriptId(void);
extern void Cinema_StopAtEnd(void);
extern s32 Cinema_IsFinished(void);

/* Retraces per frame: the mean of the last four frames without the slowest, at least `min`. */
#define SLOWEST ({ u32 _m = a; if (_m < d) _m = d; if (_m < c) _m = c; if (_m < b) _m = b; _m; })
#define MEAN ((sum - ({ u32 _t = SLOWEST; _t - 2; })) / 3)

static __inline__ u32 frameRetraces(u32 min) {
    u32 a = D_8011B09C;
    u32 b = D_8011B10C;
    u32 c = D_800F6CA8;
    u32 d = D_8011AD14;
    u32 sum = b + c + d + a;

    return MEAN < min ? min : MEAN;
}

/* Runs one frame of the level: timing, draw, then the pause/attract checks. */
void CEngineApp__Update(CEngineApp *level) {
    u64 now;
    f32 elapsed;
    s32 quit;

    now = osGetTime();
    elapsed = (f32)((now - D_8011AD08) * 64 / 3000) * 0.000001f * 15.0f;
    D_8011AD08 = now;
    if (level->m_Settings.mode == 0xD) {
        level->m_Settings.paused = MenuPool_CountOpen(level->m_Game.m_MenuPool);
    } else {
        level->m_Settings.paused = 0;
    }
    if (D_801308F8 != 0 && Demo_GetPlayerCount() == 0 && level->m_Settings.mode == 0xB) {
        level->m_Settings.paused = 1;
    }
    switch ((u32)level->m_Settings.mode) {
    case 1:
    case 3:
    case 4:
        D_8011AD10 = frameRetraces(2);
        break;
    default:
        D_8011AD10 = frameRetraces(2);
        if (D_801198A0 >= 3) {
            D_8011AD10 = frameRetraces(3);
        }
        if (Cinema_PathPlaying() != 0) {
            if ((u32)(level->m_Settings.mode - 0xF) < 2 && D_800C2034 < 0xDF) {
                D_8011AD10 = frameRetraces(2);
            } else {
                D_8011AD10 = frameRetraces(4);
            }
        }
        break;
    }
    Demo_AdvanceMatchStream();
    D_800B6D28 = (f32)(D_8011AD10 * 15) / D_8011B0A0.hz;
    if (Cinema_PathPlaying() != 0) {
        D_800B6D28 = elapsed;
    }
    if (Demo_GetPlayerCount() != 0) {
        D_800B6D28 = (f32)(D_8011AD10 * 15) * (1.0f / 60.0f);
    }
    CCache__Advance(0);
    D_8011F140 = D_800F7070;
    if (D_801198A8 == 0) {
        D_80130940 = 0;
    }
    CEngineApp__Draw(level);
    level->m_Settings.modeTimer += D_800B6D28;
    CEngineApp__UpdatePlayTime(level);
    if (MenuPool_CountOpen(D_80119E2C) == 0 && Cinema_PathPlaying() != 0 && Cinema_GetScriptId() == 0x78) {
        if (D_8011ACF4 == 0 && level->m_Settings.paused == 0) {
            D_8011ACF4 = 1;
            Cinema_StopAtEnd();
        }
    }
    if (D_8011ACF4 != 0 && Cinema_IsFinished() != 0) {
        quit = 1;
    } else {
        quit = 0;
    }
    if (quit && level->m_Settings.mode != 0xF && level->m_Settings.nextMode != 0xF) {
        level->m_Settings.fadeMode = 2;
        level->m_Settings.nextMode = 0xF;
    }
}
