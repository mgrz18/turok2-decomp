#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu.h"
#include "globals/frame.h"
#include "gfx.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "globals/framebuffer.h"
#include "protos/ultra.h"

/* gEngineApp_m_Settings_match (GameMatch in structs/game.h) as a local array view, under an
 * alias of the symbol: through it the compiler bases both stores on
 * gEngineApp_m_Settings_match + 0x44, as the ROM does. */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} Unk8011ACB0;

extern u8 gScheduler[];
extern Unk8011ACB0 D_8011ACB0_arr[] __asm__("gEngineApp_m_Settings_match");

extern void memset(void *, s32, sptr);
extern void osCreateScheduler(void *, void *, s32, s32, s32);
extern void osScAddClient(void *, void *, void *);
extern void CEngineApp__InitFade(CEngineApp *);
extern void guMtxIdent(void *);
extern void SavedPlayers_Clear(void);
extern void GameSettings_SetDefaults(void *);
extern void func_00430500(void *);
extern void Cinema__Initialize(void);
extern void func_004302A0(void);
extern void Light_SetDefaultDirection(void *);
extern void CSunFrameData__Construct(CFrameData *);
extern void CEngineApp__AdvanceFrameData(CEngineApp *);
extern void Sound__Initialize(void);

void CEngineApp__Construct(CEngineApp *state) {
    u32 i;
    u32 prev;
    u32 next;

    osCreateMesgQueue(&gGfxFrameMessageQueue, gGfxFrameMessageBuffer, 0x1000);
    memset(gSchedulerStack, 7, 0x2000);
    osCreateScheduler(gScheduler, gSchedulerStack + 0x2000, gThreadPriorityScheduler, 0, 1);
    osScAddClient(gScheduler, &gGfxClient, &gGfxFrameMessageQueue);
    state->m_Settings.prevMode = -1;
    CEngineApp__InitFade(state);
    gDList = NULL;
    guMtxIdent(&gIdentityMtx);
    if (gWarmBoot != 0) {
        SavedPlayers_Clear();
        GameSettings_SetDefaults(&gEngineApp_m_Settings);
    }
    func_00430500(&state->m_Game.m_Automap);
    i = 0;
    Cinema__Initialize();
    func_004302A0();
    Light_SetDefaultDirection(&state->m_Settings.m_Sun);
    state->m_Settings.unk1C8 = 0;
    state->m_Settings.ready = 1;
    state->m_Settings.unk1C4 = 0;
    state->m_Settings.paused = 0;
    D_8011ACB0_arr->unk44 = 0;
    D_8011ACB0_arr->unk48 = 0;
    gCameraPoolHideViews = 0;
    for (; i < gTotalFramebuffers; i++) {
        prev = (i - 1 + gTotalFramebuffers) % gTotalFramebuffers;
        next = (i + 1 + gTotalFramebuffers) % gTotalFramebuffers;
        state->m_FrameData[i].m_Msg.m_Type = 2;
        state->m_FrameData[i].m_pFrameBuffer = gCFB[i];
        state->m_FrameData[i].m_pPrev = &state->m_FrameData[prev];
        state->m_FrameData[i].m_pNext = &state->m_FrameData[next];
        CSunFrameData__Construct(&state->m_FrameData[i]);
    }
    CEngineApp__AdvanceFrameData(state);
    Sound__Initialize();
}
