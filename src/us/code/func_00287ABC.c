/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00292910.c, func_00292910, with Turok 2's layouts. */
#include "common.h"
#include "structs/game.h"
#include "structs/frame.h"
#include "globals/game.h"
#include "globals/frame.h"
#include "globals/render.h"

typedef struct VideoRate {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ f32 hz;
} VideoRate;

extern u32 gNumTicks3;          /* retrace counts of the last four frames */
extern u32 gNumTicks0;
extern u32 D_800F6CA8;
extern u32 gNumTicks2;
extern u32 gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern s32 gInSinglePlayerMenu; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */
extern VideoRate gCurrentFB;
extern f32 gFrameIncrement;          /* frame time */
extern s32 gCameraPoolHideViews; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */
extern u8 gEngineApp_m_Game_m_MenuPool[]; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */

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
    u32 a = gNumTicks3;
    u32 b = gNumTicks0;
    u32 c = D_800F6CA8;
    u32 d = gNumTicks2;
    u32 sum = b + c + d + a;

    return MEAN < min ? min : MEAN;
}

/* Runs one frame of the level: timing, draw, then the pause/attract checks. */
void CEngineApp__Update(CEngineApp *level) {
    u64 now;
    f32 elapsed;
    s32 quit;

    now = osGetTime();
    elapsed = (f32)((now - gLastEngineUpdate) * 64 / 3000) * 0.000001f * 15.0f;
    gLastEngineUpdate = now;
    if (level->m_Settings.mode == 0xD) {
        level->m_Settings.paused = MenuPool_CountOpen(level->m_Game.m_MenuPool);
    } else {
        level->m_Settings.paused = 0;
    }
    if (gInSinglePlayerMenu != 0 && Demo_GetPlayerCount() == 0 && level->m_Settings.mode == 0xB) {
        level->m_Settings.paused = 1;
    }
    switch ((u32)level->m_Settings.mode) {
    case 1:
    case 3:
    case 4:
        gNextTicks = frameRetraces(2);
        break;
    default:
        gNextTicks = frameRetraces(2);
        if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount >= 3) {
            gNextTicks = frameRetraces(3);
        }
        if (Cinema_PathPlaying() != 0) {
            if ((u32)(level->m_Settings.mode - 0xF) < 2 && gScreenHeight < 0xDF) {
                gNextTicks = frameRetraces(2);
            } else {
                gNextTicks = frameRetraces(4);
            }
        }
        break;
    }
    Demo_AdvanceMatchStream();
    gFrameIncrement = (f32)(gNextTicks * 15) / gCurrentFB.hz;
    if (Cinema_PathPlaying() != 0) {
        gFrameIncrement = elapsed;
    }
    if (Demo_GetPlayerCount() != 0) {
        gFrameIncrement = (f32)(gNextTicks * 15) * (1.0f / 60.0f);
    }
    CCache__Advance(0);
    gFrameData = gEngineApp_m_pCurrentFrameData;
    if (gEngineApp_m_Game.m_PlayerList.m_bCinemaStarted == 0) {
        gCameraPoolHideViews = 0;
    }
    CEngineApp__Draw(level);
    level->m_Settings.modeTimer += gFrameIncrement;
    CEngineApp__UpdatePlayTime(level);
    if (MenuPool_CountOpen(gEngineApp_m_Game_m_MenuPool) == 0 && Cinema_PathPlaying() != 0 && Cinema_GetScriptId() == 0x78) {
        if (gEngineApp_m_Settings_match_unk44 == 0 && level->m_Settings.paused == 0) {
            gEngineApp_m_Settings_match_unk44 = 1;
            Cinema_StopAtEnd();
        }
    }
    if (gEngineApp_m_Settings_match_unk44 != 0 && Cinema_IsFinished() != 0) {
        quit = 1;
    } else {
        quit = 0;
    }
    if (quit && level->m_Settings.mode != 0xF && level->m_Settings.nextMode != 0xF) {
        level->m_Settings.fadeMode = 2;
        level->m_Settings.nextMode = 0xF;
    }
}
