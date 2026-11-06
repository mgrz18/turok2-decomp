/* FAKEMATCH: case 4 duplicates its body under an always-true test to stop GCC keeping the constant 2 in $s1 (see there). */
/* The engine thread: boot-time setup, then the message loop (RSP task done / retrace / pre-NMI).
 * Same role as Turok: Rage Wars' func_00291404 (thejunkman9000/Turok-Rage-Wars-Decomp, src/game/func_00291404.c). */

#include "common.h"
#include "structs/game.h"

typedef struct {
    /* 0x00 */ s16 type;
} EngineMsg;

extern s32 D_80000300;          /* osTvType */
extern u8 D_80130A58[];
extern void *D_80130A88[];      /* frame buffers */
extern f32 D_8011B0A0[];
extern EngineMsg *D_8011F124;
extern s32 D_8011B110;
extern s32 D_8011AD88;
extern s32 D_8011B10C;
extern s32 D_800F6CA8;
extern s32 D_8011AD14;
extern s32 D_8011B09C;
extern s32 D_8011F130;
extern s32 D_8011F134;
extern s32 D_8011F138;
extern s32 D_800F6CA4;
extern s32 D_8011B098;
extern u32 D_800C2210;          /* frame buffer count */
extern s32 D_800C2030;          /* width */
extern s32 D_800C2034;          /* height */
extern u8 gScene[];
extern u8 D_800B6CF1;
extern s32 D_8011F13C;
extern s32 D_8011F148;
extern s32 D_8011B108;
extern s32 D_8011F144;
extern s32 D_8011B114;
extern f32 D_8011AD40;
extern u8 D_8011AD28[];         /* the engine message queue */
extern u8 D_800B75F0;

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
    GfxPool_Clear(D_80130A58);
    GfxPool_Alloc(D_80130A58);
    osViSwapBuffer(D_80130A88[0]);
    osViBlack(0);
    if (D_80000300 == 0) {
        D_8011B0A0[1] = 50.0f;
    } else {
        D_8011B0A0[1] = 60.0f;
    }
    D_8011F124 = NULL;
    Demo_Reset();
    D_8011B110 = 2;
    D_8011AD88 = 0;
    D_8011B10C = 0;
    D_800F6CA8 = 0;
    D_8011AD14 = 0;
    D_8011B09C = 0;
    D_8011F130 = 0;
    D_8011F134 = 0;
    D_8011F138 = 0;
    D_800F6CA4 = 0;
    D_8011B0A0[0] = 0;
    D_8011B098 = 0;
    CEngineApp__Construct(e);
    for (i = 0; i < D_800C2210; i++) {
        memset(D_80130A88[i], 0, D_800C2030 * D_800C2034 * 2);
    }
    CCameraPool__Construct((u8 *)e + 0x22C00);
    CPlayerPool__Construct((u8 *)e + 0x22BC0);
    CCollisionInfo__Reset();
    e->m_CartridgeIndex = 0xFCBFB4;
    CCameraPool__NewGame((u8 *)e + 0x22C00, 0);
    CPlayerPool__NewGame((u8 *)e + 0x22BC0, 0, 0);
    CScene__Construct(gScene, e->m_CartridgeIndex);
    if (D_800B6CF1 == 0) {
        Cont_InitAll();
    }
    COnScreen__Construct((u8 *)e + 0x19440);
    Pak_AllocDirectory();
    D_8011F13C = 0;
    D_8011F148 = 0;
    D_8011B108 = 0;
    D_8011F144 = 1;
    D_8011B114 = 0;
    D_8011AD40 = 120.0f;
    Controller_WatchAllPaks();
    if (SavedPlayers_IsActive()) {
        CEngineApp__SetMode(e, 13);
    } else {
        CEngineApp__SetMode(e, 0);
    }

    for (;;) {
        osRecvMesg(D_8011AD28, &D_8011F124, 1);
        switch (D_8011F124->type) {
        case 1:
            CEngineApp__Retrace(e);
            break;
        case 2:
            if ((u8 *)D_8011F124 == (u8 *)e + 0xF0) {
                D_8011F140 = (CFrameData *)e;
            } else if ((u8 *)D_8011F124 == (u8 *)e + 0x230) {
                D_8011F140 = (CFrameData *)((u8 *)e + 0x140);
            } else if ((u8 *)D_8011F124 == (u8 *)e + 0x370) {
                D_8011F140 = (CFrameData *)((u8 *)e + 0x280);
            }
            if (e->m_Settings.mode == 17) {
                D_8011B110++;
            } else {
                CEngineApp__Update(e);
            }
            break;
        case 4:
            /* FAKEMATCH: the test is always true and both branches are the same (cross-jumping
             * merges them); without it GCC also keeps the case constant 2 in $s1 across the loop
             * (12 words differ). Found by the permuter; no natural form found. */
            if (D_8011F124 != NULL) {
                D_800B75F0 = 0;
                Controller_ResetAllRumble();
                Controller_SetRumbleOptionAll(0);
                e->m_Settings.fastFade = 1;
                CEngineApp__SetupFadeTo(e, 17);
                e->m_Settings.unk1C8 = 1;
                break;
            } else {
                D_800B75F0 = 0;
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
