/* FAKEMATCH: case 4 duplicates its body under an always-true test to stop GCC keeping the constant 2 in $s1 (see there). */
/* The engine thread: boot-time setup, then the message loop (RSP task done / retrace / pre-NMI).
 * Same role as Turok: Rage Wars' func_00291404 (thejunkman9000/Turok-Rage-Wars-Decomp, src/game/func_00291404.c). */

#include "common.h"
#include "structs/game.h"
#include "globals/pak.h"
#include "globals/frame.h"
#include "globals/scene.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "globals/controller.h"
#include "globals/libultra.h"

typedef struct {
    /* 0x00 */ s16 type;
} EngineMsg;

extern u8 gPoolBuffers[];
extern void *gCFB[];      /* frame buffers */
extern f32 gCurrentFB[];
extern EngineMsg *gGfxMsg;
extern s32 gCurrentTick;
extern s32 gNumTicks0;
extern s32 D_800F6CA8;
extern s32 gNumTicks2;
extern s32 gNumTicks3;
extern s32 gCurrentTickWaiting;
extern s32 D_8011F144;
extern s32 D_8011B114;
extern f32 gPakCheckTimer;
extern u8 gGfxFrameMessageQueue[];         /* the engine message queue */

extern void InitializeSystemResources(void);
extern void osCreateViManager(s32 pri);
extern void osViBlack(s32 black);           /* osViBlack */
extern void GfxPool_Clear(void *p);
extern void GfxPool_Alloc(void *p);
extern void osViSwapBuffer(void *frameBuffer);   /* osViSwapBuffer */
extern void Demo_Reset(void);
extern void CEngineApp__Construct(CEngineApp *e);
extern void memset(void *p, s32 c, s32 n);   /* memset */
extern void CCameraPool__Construct(void *p);
extern void CPlayerPool__Construct(void *p);
extern void CCollisionInfo__Reset(void);
extern void CCameraPool__NewGame(void *p, s32 a);
extern void CPlayerPool__NewGame(void *p, s32 a, s32 b);
extern void CScene__Construct(void *p, s32 seed);
extern void Cont_InitAll(void);
extern void COnScreen__Construct(void *p);
extern void Pak_AllocDirectory(void);
extern void Controller_WatchAllPaks(void);
extern s32 SavedPlayers_IsActive(void);
extern void CEngineApp__SetMode(CEngineApp *e, s32 mode);
extern s32 osRecvMesg(void *mq, void *msg, s32 flag);
extern void CEngineApp__Retrace(CEngineApp *e);
extern void CEngineApp__Update(CEngineApp *e);
extern void Controller_ResetAllRumble(void);
extern void Controller_SetRumbleOptionAll(s32 a);
extern void CEngineApp__SetupFadeTo(CEngineApp *e, s32 mode);

void CEngineApp__Main(CEngineApp *e) {
    u32 i;

    InitializeSystemResources();
    osCreateViManager(0xFE);
    osViBlack(1);
    GfxPool_Clear(gPoolBuffers);
    GfxPool_Alloc(gPoolBuffers);
    osViSwapBuffer(gCFB[0]);
    osViBlack(0);
    if (osTvType == 0) {
        gCurrentFB[1] = 50.0f;
    } else {
        gCurrentFB[1] = 60.0f;
    }
    gGfxMsg = NULL;
    Demo_Reset();
    gDisplayListCount = 2;
    gCurrentTick = 0;
    gNumTicks0 = 0;
    D_800F6CA8 = 0;
    gNumTicks2 = 0;
    gNumTicks3 = 0;
    gNumTicksPending = 0;
    gCurrentTickWaiting = 0;
    gNumTicksWaiting = 0;
    gNextFrameToShow = 0;
    gCurrentFB[0] = 0;
    gPendingFB = 0;
    CEngineApp__Construct(e);
    for (i = 0; i < gTotalFramebuffers; i++) {
        memset(gCFB[i], 0, gScreenWidth * gScreenHeight * 2);
    }
    CCameraPool__Construct((u8 *)e + 0x22C00);
    CPlayerPool__Construct((u8 *)e + 0x22BC0);
    CCollisionInfo__Reset();
    e->m_CartridgeIndex = 0xFCBFB4;
    CCameraPool__NewGame((u8 *)e + 0x22C00, 0);
    CPlayerPool__NewGame((u8 *)e + 0x22BC0, 0, 0);
    CScene__Construct(&gScene, e->m_CartridgeIndex);
    if (gValidControllerBits == 0) {
        Cont_InitAll();
    }
    COnScreen__Construct((u8 *)e + 0x19440);
    Pak_AllocDirectory();
    gPakCheckHasPak = 0;
    gPakCheckDone = 0;
    gPakMenuStartPressed = 0;
    D_8011F144 = 1;
    D_8011B114 = 0;
    gPakCheckTimer = 120.0f;
    Controller_WatchAllPaks();
    if (SavedPlayers_IsActive()) {
        CEngineApp__SetMode(e, 13);
    } else {
        CEngineApp__SetMode(e, 0);
    }

    for (;;) {
        osRecvMesg(gGfxFrameMessageQueue, &gGfxMsg, 1);
        switch (gGfxMsg->type) {
        case 1:
            CEngineApp__Retrace(e);
            break;
        case 2:
            if ((u8 *)gGfxMsg == (u8 *)e + 0xF0) {
                gFrameData = (CFrameData *)e;
            } else if ((u8 *)gGfxMsg == (u8 *)e + 0x230) {
                gFrameData = (CFrameData *)((u8 *)e + 0x140);
            } else if ((u8 *)gGfxMsg == (u8 *)e + 0x370) {
                gFrameData = (CFrameData *)((u8 *)e + 0x280);
            }
            if (e->m_Settings.mode == 17) {
                gDisplayListCount++;
            } else {
                CEngineApp__Update(e);
            }
            break;
        case 4:
            /* FAKEMATCH: the test is always true and both branches are the same (cross-jumping
             * merges them); without it GCC also keeps the case constant 2 in $s1 across the loop
             * (12 words differ). Found by the permuter; no natural form found. */
            if (gGfxMsg != NULL) {
                gBootSignature[0] = 0;
                Controller_ResetAllRumble();
                Controller_SetRumbleOptionAll(0);
                e->m_Settings.fastFade = 1;
                CEngineApp__SetupFadeTo(e, 17);
                e->m_Settings.unk1C8 = 1;
                break;
            } else {
                gBootSignature[0] = 0;
                Controller_ResetAllRumble();
                Controller_SetRumbleOptionAll(0);
                e->m_Settings.fastFade = 1;
                CEngineApp__SetupFadeTo(e, 17);
                e->m_Settings.unk1C8 = 1;
                break;
            }
        }
    }
}
