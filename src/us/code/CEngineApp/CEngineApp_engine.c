/* Boundary fixed in versions/cuts.us.txt: splat starts this function 2 words early (0x285CE8: never-executed
 * `swc1 $f21,0x10($sp); sw $s0,0x14($sp)`); the real entry is 0x285CF0, as the
 * pointer `.word 0x00285CF0` in us/asm/data/AB858.rodata.s shows. */
#include "common.h"
#include "structs/game.h"
#include "globals/scene.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "protos/scene.h"
#include "common.h"
#include "gfx.h"
#include "structs/frame.h"
#include "globals/menu.h"
#include "globals/text.h"
#include "globals/game.h"
#include "protos/player.h"
#include "globals/frame.h"
#include "globals/framebuffer.h"
#include "protos/ultra.h"
/* FAKEMATCH: case 4 duplicates its body under an always-true test to stop GCC keeping the constant 2 in $s1 (see there). */
/* The engine thread: boot-time setup, then the message loop (RSP task done / retrace / pre-NMI).
 * Same role as Turok: Rage Wars' func_00291404 (thejunkman9000/Turok-Rage-Wars-Decomp, src/game/func_00291404.c). */

#include "common.h"
#include "globals/pak.h"
#include "globals/controller.h"
#include "globals/libultra.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002917E8.c, func_002917E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "gbi_t2.h"
#include "globals/ucode.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/render_screen_overlay.CPP, Render_DrawTextLines, with Turok 2's layouts. */
#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00292318.c, func_00292318, with Turok 2's layouts. */
#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00292910.c, func_00292910, with Turok 2's layouts. */
#include "common.h"
/* The port: the same through CEngineApp / CFrameData (structs/game.h, frame.h). */
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80291074_de_8.c, func_8029311C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "m2c_macros.h"
/* FAKEMATCH (round 21 audit): an if/else whose two arms are the same code, kept
 * for the ROM's register allocation or layout; with one arm: CEngineApp__UpdateFade (two sites): 101-103 of
 * 116 words. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002935A4.c, func_002935A4, with Turok 2's layouts. */
#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00293378.c, func_00293378.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80291054_10.CPP, func_80293904_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/controller.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80293A04.CPP, func_80293A20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00293F38.c, func_00293F38.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80293A04_3.CPP, func_80294158_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/render.h"
#include "structs/player.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00294BB0.c, func_00294BB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/rumble.h"

extern u8 gEngineApp_m_Game_m_CameraPool_m_MainCamera[]; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */
extern void CCamera__DisplayListSetup(void *);
extern void CMtxF__SetTranslate(f32 *matrix, f32 x, f32 y, f32 z);
extern void CMtxF__PreRotateY(f32 *matrix, f32 angle);
extern void CMtxF__SwapYZRows(f32 *matrix);
extern void CMtxF__ToMtxAffine(f32 *src, f32 *dst);
extern void Render_ResetBatches(void);
extern void Render_QueueModelByKey(sptr key, void *matrix, void *lights, s32 lod, s32 pass);
extern void Render_FlushBatches(void);
void Render_DrawTextLines(void *state, s32 drawBackdrop, char **lines, s32 x, s32 y, s32 lineStep, f32 scaleX, f32 scaleY);

void EngineMode03_ExpansionPak_Draw(CEngineApp *view) {
    f32 mtx[16];
    s32 id;
    s32 key;

    gFogEnabled = 0;
    id = CScene__FindObjectType(&gScene, 0xE10);
    if (id == -1) {
        return;
    }
    CCamera__DisplayListSetup(gEngineApp_m_Game_m_CameraPool_m_MainCamera);
    CMtxF__SetTranslate(mtx, 0.0f, -100.0f, -500.0f);
    CMtxF__PreRotateY(mtx, view->m_Settings.modeTimer * 0.17453295f);
    CMtxF__SwapYZRows(mtx);
    CMtxF__ToMtxAffine(mtx, sExpansionPakMtx[gEvenOdd]);
    key = CScene__GetObject(&gScene, id);
    Render_ResetBatches();
    Render_QueueModelByKey(key, sExpansionPakMtx[gEvenOdd], &gDefaultLights, 0, -1);
    Render_FlushBatches();
    Render_DrawTextLines(view, 0, sExpansionPakText, gScreenWidth / 2, 0xBE, 0xA, 0.8f, 1.0f);
}
void CEngineApp__ClearDepthRect(sptr unused, s32 x0, s32 x1, s32 y0, s32 y1);
extern void Hud_GetSpriteSize(s32, s32, s32 *, s32 *);
extern void Hud_SetupTextureMode(s32);
extern void Hud_DrawSprite(s32, s32, s32, s32, f32, f32, s32);

void EngineMode07_Title_Draw(sptr arg0) {
    CFrameData *fb;
    s32 width;
    s32 height;
    Gfx *g;

    fb = gEngineApp_m_pCurrentFrameData;
    CEngineApp__ClearDepthRect(arg0, 0, gScreenWidth, 0, gScreenHeight);
    width = 0;
    height = 0;
    /* gDPSetScissor(G_SC_NON_INTERLACE, 0, 0, w - 1, h - 1) */
    g = gDList++;
    g->w0 = 0xED000000;
    g->w1 = (((s32)((f32)(gScreenWidth - 1) * 4.0f) & 0xFFF) << 12) | ((s32)((f32)(gScreenHeight - 1) * 4.0f) & 0xFFF);
    /* gDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, w, depth buffer) */
    g = gDList++;
    g->w0 = 0xFF100000 | ((gScreenWidth - 1) & 0xFFF);
    g->w1 = fb->m_pFrameBuffer;
    /* gDPSetCycleType(G_CYC_FILL) */
    g = gDList++;
    g->w0 = 0xE3000A01;
    g->w1 = 0x300000;
    /* gDPSetCombineMode(G_CC_SHADE, G_CC_SHADE) */
    g = gDList++;
    g->w0 = 0xFCFFFFFF;
    g->w1 = 0xFFFE793C;
    /* gDPSetRenderMode(0, 0) */
    g = gDList++;
    g->w0 = 0xE200001C;
    g->w1 = 0;
    /* gDPSetFillColor: max depth */
    g = gDList++;
    g->w0 = 0xF7000000;
    g->w1 = 0x10001;
    /* gDPFillRectangle(0, 0, w, h) */
    g = gDList++;
    g->w0 = 0xF6000000 | ((gScreenWidth & 0x3FF) << 14) | ((gScreenHeight & 0x3FF) << 2);
    g->w1 = 0;
    Hud_GetSpriteSize(0x386, 0, &width, &height);
    if (width != 0 && height != 0) {
        Hud_SetupTextureMode(0xFF);
        Hud_DrawSprite(0x386, 0, 0, 0, (f32)gScreenWidth / (f32)width, (f32)gScreenHeight / (f32)height, 1);
    }
}

extern void COnScreen__BeginText(void);
extern void COnScreen__SelectTinyFont(void);
extern void COnScreen__SetFontScale(f32, f32);
extern void COnScreen__SetFontColor(s32, s32, s32, s32, s32, s32);
extern void COnScreen__DrawTextShadowed(char *, f32, f32, f32, f32, s32, s32, s32);
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Draws the scrolling credits, fading lines in at the top of the screen. */
void Credits_Draw(void) {
    s32 line;
    f32 y;
    f32 alpha;
    char *text;
    s32 a;

    line = sCreditsLine;
    y = gCreditsScroll;
    COnScreen__BeginText();
    COnScreen__SelectTinyFont();
    COnScreen__SetFontScale(1.0f, 1.0f);
    do {
        text = gCreditsLines[line];
        if (text != NULL) {
            alpha = MIN(MAX(y, 0.0f), 16.0f) * 16.0f;
            if (alpha > 255.0f) {
                alpha = 255.0f;
            }
            a = alpha;
            COnScreen__SetFontColor(0xFF, 0xFF, 0xFF, a, a, a);
            if (y >= 0.0f) {
                COnScreen__DrawTextShadowed(text, (gScreenWidth * 14) / 284, (s32)y, gScreenWidth * 0.0028169014f,
                              gScreenHeight * 0.0045045046f, 1, a, 4);
            }
            line++;
        }
        y += gScreenHeight * 0.045045048f;
    } while (y <= gScreenHeight);
}
void CEngineApp__StartGameAtWarp(CEngineApp *arg0, s32 arg1);
void CEngineApp__SetupFadeTo(CEngineApp *arg0, s32 arg1);
extern void Sound__PlaySpeech(s32);

/* Scrolls the credits up; once past the last line, waits and leaves. */
void Credits_Update(sptr arg0) { /* the engine pointer-wide (sptr: s32 on the N64; the host cut it: crash after the credits) */
    f32 spacing;

    if (gCreditsDone != 0) {
        gCreditsTimeLeft -= gFrameIncrement;
        if (gCreditsTimeLeft <= 0.0f) {
            CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
            if (gEndingPrimagenDefeated != 0) {
                if (gFinalEnding == 0) {
                    CEngineApp__StartGameAtWarp(arg0, 0x1B57);
                    return;
                }
            } else if (gFinalEnding == 0) {
                return;
            }
            CEngineApp__SetupFadeTo(arg0, 1);
            gEngineApp_m_Settings.match.unk48 = 1;
        }
        return;
    }
    if (gScreenHeight >= 223) {
        gCreditsScroll -= 1.0f;
    } else {
        gCreditsScroll -= 0.5f;
    }
    spacing = gScreenHeight * 0.045045048f;
    if (gCreditsScroll <= -spacing) {
        sCreditsLine++;
        gCreditsScroll += spacing;
        if (gCreditsLines[sCreditsLine] == NULL) {
            gCreditsDone = 1;
            Sound__PlaySpeech(0x266);
            gCreditsTimeLeft = 67.5f;
        }
    }
}

/* gEngineApp_m_Settings_match (GameMatch in structs/game.h) as a local array view, under an
 * alias of the symbol: through it the compiler bases both stores on
 * gEngineApp_m_Settings_match + 0x44, as the ROM does. */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} Unk8011ACB0;
extern Unk8011ACB0 sSettingsMatchView[] __asm__("gEngineApp_m_Settings_match");
extern void *memset(void *dst, s32 value, uptr size);
extern void osCreateScheduler(void *, void *, s32, s32, s32);
extern void osScAddClient(void *, void *, void *);
void CEngineApp__InitFade(CEngineApp *arg0);
extern void guMtxIdent(void *);
extern void SavedPlayers_Clear(void);
extern void GameSettings_SetDefaults(void *);
extern void Automap_ClearBuffers(void *);
extern void Cinema__Initialize(void);
extern void Render_InitUnitQuad(void);
extern void Light_SetDefaultDirection(void *);
extern void CSunFrameData__Construct(CFrameData *);
void CEngineApp__AdvanceFrameData(CEngineApp *gfx);
extern void Sound__Initialize(void);

void CEngineApp__Construct(CEngineApp *state) {
    u32 i;
    u32 prev;
    u32 next;

    osCreateMesgQueue(&gGfxFrameMessageQueue, gGfxFrameMessageBuffer, 0x1000);
    memset(gSchedulerStack, 7, 0x2000);
    osCreateScheduler(&gScheduler, gSchedulerStack + 0x2000, gThreadPriorityScheduler, 0, 1);
    osScAddClient(&gScheduler, &gGfxClient, &gGfxFrameMessageQueue);
    state->m_Settings.prevMode = -1;
    CEngineApp__InitFade(state);
    gDList = NULL;
    guMtxIdent(&gIdentityMtx);
    if (gWarmBoot != 0) {
        SavedPlayers_Clear();
        GameSettings_SetDefaults(&gEngineApp_m_Settings);
    }
    Automap_ClearBuffers(&state->m_Game.m_Automap);
    i = 0;
    Cinema__Initialize();
    Render_InitUnitQuad();
    Light_SetDefaultDirection(&state->m_Settings.m_Sun);
    state->m_Settings.unk1C8 = 0;
    state->m_Settings.ready = 1;
    state->m_Settings.unk1C4 = 0;
    state->m_Settings.paused = 0;
    sSettingsMatchView->unk44 = 0;
    sSettingsMatchView->unk48 = 0;
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

extern u8 gPoolBuffers[];
extern void InitializeSystemResources(void);
extern void osCreateViManager(s32 pri);
extern void osViBlack(s32 black);           /* osViBlack */
extern void GfxPool_Clear(void *p);
extern void GfxPool_Alloc(void *p);
extern void Demo_Reset(void);
extern void CCameraPool__Construct(void *p);
extern void CPlayerPool__Construct(void *p);
extern void CCollisionInfo__Reset(void);
extern void CCameraPool__NewGame(void *owner, s32 count);
extern void CPlayerPool__NewGame(void *list, s32 count, s32 arg2);
extern void Cont_InitAll(void);
extern void COnScreen__Construct(void *p);
extern void Pak_AllocDirectory(void);
extern void Controller_WatchAllPaks(void);
extern s32 SavedPlayers_IsActive(void);
void CEngineApp__SetMode(CEngineApp *arg0, s32 arg1);
void CEngineApp__Retrace(CEngineApp *arg0);
void CEngineApp__Update(CEngineApp *level);
extern void Controller_ResetAllRumble(void);
extern void Controller_SetRumbleOptionAll(s32 a);

#ifndef TARGET_PORT /* the port uses the literals */
/* mainproc's build stamp strings: the ROM has them here, before CEngineApp__Main's literals, though
 * mainproc comes later in .text (an earlier expansion wrote them in the original) */
const char sEngineStampFmt[] = "%s %s";
const char sEngineStampDate[] = "Nov 10 1998";
const char sEngineStampTime[] = "13:17:04";
#endif

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
        gRefreshRate = 50.0f;
    } else {
        gRefreshRate = 60.0f;
    }
    gGfxMsg = NULL;
    Demo_Reset();
    gDisplayListCount = 2;
    gCurrentTick = 0;
    gNumTicks0 = 0;
    gNumTicks1 = 0;
    gNumTicks2 = 0;
    gNumTicks3 = 0;
    gNumTicksPending = 0;
    gCurrentTickWaiting = 0;
    gNumTicksWaiting = 0;
    gNextFrameToShow = 0;
    gCurrentFB = NULL;
    gPendingFB = NULL;
    CEngineApp__Construct(e);
    for (i = 0; i < gTotalFramebuffers; i++) {
        memset(gCFB[i], 0, gScreenWidth * gScreenHeight * 2);
    }
    CCameraPool__Construct(e->m_Game.m_CameraPool);
    CPlayerPool__Construct(&e->m_Game.m_PlayerList);
    CCollisionInfo__Reset();
    e->m_CartridgeIndex = 0xFCBFB4;
    CCameraPool__NewGame(e->m_Game.m_CameraPool, 0);
    CPlayerPool__NewGame(&e->m_Game.m_PlayerList, 0, 0);
    CScene__Construct(&gScene, e->m_CartridgeIndex);
    if (gValidControllerBits == 0) {
        Cont_InitAll();
    }
    COnScreen__Construct(e->unk19440);
    Pak_AllocDirectory();
    gPakCheckHasPak = 0;
    gPakCheckDone = 0;
    gPakMenuStartPressed = 0;
    gPakBootPending = 1;
    gPakMenuConfirmed = 0;
    gPakCheckTimer = 120.0f;
    Controller_WatchAllPaks();
    if (SavedPlayers_IsActive()) {
        CEngineApp__SetMode(e, 13);
    } else {
        CEngineApp__SetMode(e, 0);
    }

    for (;;) {
        osRecvMesg(&gGfxFrameMessageQueue, &gGfxMsg, 1);
        switch (gGfxMsg->type) {
        case 1:
            CEngineApp__Retrace(e);
            break;
        case 2:
            if (gGfxMsg == (OSScMsg *) &e->m_FrameData[0].m_Msg) {
                gFrameData = &e->m_FrameData[0];
            } else if (gGfxMsg == (OSScMsg *) &e->m_FrameData[1].m_Msg) {
                gFrameData = &e->m_FrameData[1];
            } else if (gGfxMsg == (OSScMsg *) &e->m_FrameData[2].m_Msg) {
                gFrameData = &e->m_FrameData[2];
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
             * (12 words differ). Found by the permuter; no natural form found. Why it works:
             * loop.c hoists an invariant when threshold * savings * lifetime >= the loop's insn
             * count, and the copy only makes the loop longer at loop time (any three or so more
             * insns do, e.g. a dead `i = i * 3 + 1;`). The original must have had a little more
             * code in the loop that a later pass removes; Turok 3's version (references/turok3
             * tengine.c) adds an options check in case 2, which Turok 2's ROM does not have. */
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

extern B21Vp gScreenViewport[];

void CEngineApp__BeginDisplayList(void)
{
  f32 new_var;
  s32 new_var2;
  s32 new_var3;
  DlCmd *d1;
  DlCmd *d2;
  DlCmd *d3;
  DlCmd *d4;
  DlCmd *d5;
  DlCmd *d6;
  B21Vp *vp;
  gDList = gEngineApp_m_pCurrentFrameData->m_pDisplayList;
  d1 = gDList++;
  d1->w0 = 0xDB060000;
  d1->w1 = 0;
  d2 = gDList++;
  d2->w0 = 0xDE000000;
  d2->w1 = GBI_ADDR(gRspInitDL);
  d3 = gDList++;
  d3->w0 = 0xDE000000;
  d3->w1 = GBI_ADDR(gRdpInitDL);
  d4 = gDList++;
  d4->w0 = 0xFE000000;
  vp = &gScreenViewport[gEvenOdd];
  d4->w1 = gDepthBuffer;
  vp->vscale[1] = gScreenHeight * 2;
  vp->vscale[0] = gScreenWidth * 2;
  vp->vscale[2] = 0x3FF;
  vp->vtrans[1] = gScreenHeight * 2;
  vp->vscale[2] = 0x3FF;
  vp->vscale[3] = 0;
  vp->vtrans[0] = gScreenWidth * 2;
  vp->vtrans[2] = 0;
  vp->vtrans[3] = 0;
  new_var = (f32) (gScreenWidth - 1);
  d5 = gDList++;
  d5->w0 = 0xDC080008;
  d5->w1 = GBI_ADDR(vp);
  d6 = gDList++;
  d6->w0 = 0xED000000;
  d6->w1 = ((((s32) (new_var * 4.0f)) & 0xFFF) << 12) | (((s32) (((f32) (gScreenHeight - 1)) * 4.0f)) & 0xFFF);
}

#ifdef TARGET_PORT
/* The port: the same through CFrameData (structs/frame.h) and the scheduler's OSScTask
 * (structs/sched.h, port/game/t2sched.c); the views below have the N64 layout. */
#include "structs/game.h"

extern void Sched_SubmitTask(void *, OSScTask *);

static void fillTask(OSScTask *t, CFrameData *task, void *dl, u32 size, u64 *text, u64 *data) {
    t->list.t.data_ptr = dl;
    t->list.t.type = 1;
    t->list.t.ucode_boot = rspbootTextStart;
    t->list.t.ucode_boot_size = (u8 *)rspbootTextEnd - (u8 *)rspbootTextStart;
    t->list.t.ucode_size = 0x1000;
    t->list.t.ucode_data_size = 0x800;
    t->list.t.yield_data_ptr = gGfxYieldBuffer;
    t->list.t.yield_data_size = 0xC00;
    t->list.t.ucode = text;
    t->list.t.ucode_data = data;
    t->list.t.flags = 0;
    t->list.t.dram_stack = NULL;
    t->list.t.dram_stack_size = 0;
    t->list.t.output_buff = gRspFifoBuffer;
    t->list.t.output_buff_size = (u64 *)((u8 *)gRspFifoBuffer + gRspFifoSize);
    t->list.t.data_size = size;
    t->next = NULL;
    t->msgQ = &gGfxFrameMessageQueue;
    t->msg = &task->m_Msg;
    t->framebuffer = (void *)(uptr)task->m_pFrameBuffer;
}

void CEngineApp__SubmitGfxTask(CEngineApp *gfx, CFrameData *task) {
    u32 idx;

    fillTask(&task->m_Task, task, task->m_pDisplayList, task->m_DisplayListSize,
             (u64 *)gspF3DEX_NoN_fifoTextStart, (u64 *)gspF3DEX_NoN_fifoDataStart);
    task->m_Task.flags = task->m_bLines ? 3 : 0x23;
    if (task->m_bLines) {
        fillTask(&task->m_LineTask, task, task->m_pLineList, task->m_LineListSize,
                 (u64 *)gspL3DEX_fifoTextStart, (u64 *)gspL3DEX_fifoDataStart);
        task->m_LineTask.flags = 0x23;
    }
    osWritebackDCacheAll();
    Sched_SubmitTask(&gScheduler, &task->m_Task);
    if (task->m_bLines) {
        Sched_SubmitTask(&gScheduler, &task->m_LineTask);
    }
    gFrameCount++;
    idx = gFrameCount % gTotalFramebuffers;
    gFrameCountGameplay++;
    gEvenOdd ^= 1;
    gfx->m_pCurrentFrameData = &gfx->m_FrameData[idx];
    gfx->m_FrameData[idx].m_pDisplayList = gEvenOdd == 0 ? gDisplayList1 : gDisplayList0;
    gfx->m_pCurrentFrameData->m_pLineList = gEvenOdd == 0 ? gLineList1 : gLineList0;
}
#else
typedef struct {
    /* 0x00 */ u32 type;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u64 *ucode_boot;
    /* 0x0C */ u32 ucode_boot_size;
    /* 0x10 */ u64 *ucode;
    /* 0x14 */ u32 ucode_size;
    /* 0x18 */ u64 *ucode_data;
    /* 0x1C */ u32 ucode_data_size;
    /* 0x20 */ u64 *dram_stack;
    /* 0x24 */ u32 dram_stack_size;
    /* 0x28 */ u64 *output_buff;
    /* 0x2C */ u64 *output_buff_size;
    /* 0x30 */ u64 *data_ptr;
    /* 0x34 */ u32 data_size;
    /* 0x38 */ u64 *yield_data_ptr;
    /* 0x3C */ u32 yield_data_size;
} RspTask; /* OSTask_t */

typedef struct ScTask {
    /* 0x00 */ struct ScTask *next;
    /* 0x04 */ u32 state;
    /* 0x08 */ u32 flags;
    /* 0x0C */ void *framebuffer;
    /* 0x10 */ RspTask list;
    /* 0x50 */ void *msgQ;
    /* 0x54 */ void *msg;
    /* 0x58 */ u64 startTime;
    /* 0x60 */ u64 totalTime;
} ScTask; /* OSScTask, size 0x68 */

typedef struct {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ ScTask sc[2];
    /* 0x0F0 */ s16 msg;
    /* 0x0F2 */ u8 padF2[0x110 - 0xF2];
    /* 0x110 */ void *framebuffer;
    /* 0x114 */ u64 *dl[2];
    /* 0x11C */ u32 dlSize[2];
    /* 0x124 */ u8 pad124[4];
    /* 0x128 */ s32 twoTasks;
    /* 0x12C */ u8 pad12C[0x140 - 0x12C];
} GfxTask; /* size 0x140 */

typedef struct {
    /* 0x000 */ GfxTask tasks[3];
    /* 0x3C0 */ GfxTask *cur;
} GfxState;


extern void Sched_SubmitTask(void *, ScTask *);

struct CEngineApp;
struct CFrameData;

/* Builds the RSP task(s) for a frame, hands them to the scheduler, moves to the next frame.
 * Takes the port body's parameter types (structs/game.h, structs/frame.h); this body's views as macros (a local copy changes the registers). */
#define gfx ((GfxState *) app)
#define task ((GfxTask *) frame)
void CEngineApp__SubmitGfxTask(struct CEngineApp *app, struct CFrameData *frame) {
    u32 dataSize;
    u32 dataSize2;
    ScTask *t;
    u32 idx;
    GfxTask *cur;

    t = &task->sc[0];
    t->list.data_ptr = task->dl[0];
    dataSize = task->dlSize[0];
    t->list.type = 1;
    t->list.ucode_boot = rspbootTextStart;
    t->list.ucode_boot_size = (u8 *)rspbootTextEnd - (u8 *)rspbootTextStart;
    t->list.ucode_size = 0x1000;
    t->list.ucode_data_size = 0x800;
    t->list.yield_data_ptr = gGfxYieldBuffer;
    t->list.yield_data_size = 0xC00;
    t->list.ucode = gspF3DEX_NoN_fifoTextStart;
    t->list.ucode_data = gspF3DEX_NoN_fifoDataStart;
    t->list.flags = 0;
    t->list.dram_stack = NULL;
    t->list.dram_stack_size = 0;
    t->list.output_buff = gRspFifoBuffer;
    t->list.output_buff_size = (u64 *)((u8 *)gRspFifoBuffer + gRspFifoSize);
    t->list.data_size = dataSize;
    task->sc[0].next = NULL;
    t->msgQ = &gGfxFrameMessageQueue;
    t->msg = &task->msg;
    t->framebuffer = task->framebuffer;
    if (task->twoTasks) {
        t->flags = 3;
    } else {
        t->flags = 0x23;
    }
    if (task->twoTasks) {
        t = &task->sc[1];
        t->list.data_ptr = task->dl[1];
        dataSize2 = task->dlSize[1];
        t->list.type = 1;
        t->list.ucode_boot = rspbootTextStart;
        t->list.ucode_boot_size = (u8 *)rspbootTextEnd - (u8 *)rspbootTextStart;
        t->list.ucode_size = 0x1000;
        t->list.ucode_data_size = 0x800;
        t->list.yield_data_ptr = gGfxYieldBuffer;
        t->list.yield_data_size = 0xC00;
        t->list.ucode = gspL3DEX_fifoTextStart;
        t->list.ucode_data = gspL3DEX_fifoDataStart;
        t->list.flags = 0;
        t->list.dram_stack = NULL;
        t->list.dram_stack_size = 0;
        t->list.output_buff = gRspFifoBuffer;
        t->list.output_buff_size = (u64 *)((u8 *)gRspFifoBuffer + gRspFifoSize);
        t->list.data_size = dataSize2;
        task->sc[1].next = NULL;
        t->flags = 0x23;
        t->msgQ = &gGfxFrameMessageQueue;
        t->msg = &task->msg;
        t->framebuffer = task->framebuffer;
    }
    osWritebackDCacheAll();
    Sched_SubmitTask(&gScheduler, &task->sc[0]);
    if (task->twoTasks) {
        Sched_SubmitTask(&gScheduler, &task->sc[1]);
    }
    gFrameCount++;
    idx = gFrameCount % gTotalFramebuffers;
    gFrameCountGameplay++;
    gEvenOdd ^= 1;
    gfx->cur = &gfx->tasks[idx];
    gfx->tasks[idx].dl[0] = gEvenOdd == 0 ? gDisplayList1 : gDisplayList0;
    cur = gfx->cur;
    cur->dl[1] = gEvenOdd == 0 ? gLineList1 : gLineList0;
}
#undef gfx
#undef task
#endif

extern s32 gEngineApp_m_OnScreen_m_ShadowXOff[2]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern s32 Render_SetRenderMode(u32 mode);
extern void COnScreen__SelectSmallFont();
extern void COnScreen__SetFontScale(f32 x, f32 y);
extern s32 COnScreen__DrawString(char *text, s32 x, s32 y, s32 alpha, s32, s32, f32, f32);

/* CEngineApp__ClearDepthRect: fills a rectangle of the z-buffer, then points the color image back at the frame. */
static __inline__ void clearDepth(void *unused, s32 x0, s32 x1, s32 y0, s32 y1) {
    s32 colorImage;

    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    Render_SetRenderMode(0x14);
    colorImage = 0xFF100000 | ((gScreenWidth - 1) & 0xFFF);
    {
        Gfx *g = gDList++;
        g->w0 = colorImage;     /* gDPSetColorImage(RGBA, 16b, width, z-buffer) */
        g->w1 = gDepthBuffer;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_FILL) */
        g->w1 = 0x00300000;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xF7000000;     /* gDPSetFillColor(max depth) */
        g->w1 = 0xFFFCFFFC;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xF6000000 | ((x1 & 0x3FF) << 14) | ((y1 & 0x3FF) << 2); /* gDPFillRectangle */
        g->w1 = ((x0 & 0x3FF) << 14) | ((y0 & 0x3FF) << 2);
    }
    {
        Gfx *g = gDList++;
        g->w0 = colorImage;     /* gDPSetColorImage(RGBA, 16b, width, frame) */
        g->w1 = gEngineApp.m_pCurrentFrameData->m_pFrameBuffer;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_1CYCLE) */
        g->w1 = 0x00100000;
    }
}

/* Draws a NULL-terminated list of scaled strings, over a cleared screen if asked. */
void Render_DrawTextLines(void *state, s32 drawBackdrop, char **lines, s32 x, s32 y, s32 lineStep, f32 scaleX, f32 scaleY) {
    CFrameData *task = gEngineApp.m_pCurrentFrameData;
    char *text;
    s32 i;

    y = y * gScreenHeight / 222;
    if (drawBackdrop) {
        clearDepth(state, 0, gScreenWidth, 0, gScreenHeight);
        {
            Gfx *g = gDList++;
            g->w0 = 0xED000000;     /* gDPSetScissor(0, 0, width - 1, height - 1) */
            g->w1 = (((s32)((f32)(gScreenWidth - 1) * 4.0f) & 0xFFF) << 12) | ((s32)((f32)(gScreenHeight - 1) * 4.0f) & 0xFFF);
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xFF100000 | ((gScreenWidth - 1) & 0xFFF);  /* gDPSetColorImage(frame) */
            g->w1 = task->m_pFrameBuffer;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_FILL) */
            g->w1 = 0x00300000;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xFCFFFFFF;     /* gDPSetCombineMode(shade) */
            g->w1 = 0xFFFE793C;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE200001C;     /* gDPSetRenderMode(0, 0) */
            g->w1 = 0;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xF7000000;     /* gDPSetFillColor */
            g->w1 = 0x10001;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xF6000000 | ((gScreenWidth & 0x3FF) << 14) | ((gScreenHeight & 0x3FF) << 2); /* gDPFillRectangle */
            g->w1 = 0;
        }
    }
    COnScreen__BeginText();
    COnScreen__SelectSmallFont();
    scaleX = scaleX * gScreenWidth * (1.0f / 284.0f);
    scaleY = scaleY * gScreenHeight * (1.0f / 222.0f);
    scaleX *= (gScreenHeight >= 0xDF) ? 0.8f : 0.85f;
    scaleY *= 0.95f;
    COnScreen__SetFontScale(scaleX, scaleY);
    gEngineApp_m_OnScreen_m_ShadowXOff[0] = 1;
    gEngineApp_m_OnScreen_m_ShadowXOff[1] = 1;
    for (i = 0; lines[i] != NULL; i++) {
        text = lines[i];
        COnScreen__DrawString(text, x, y, 0xFF, 1, 1, 1.0f, 1.0f);
        y = y + lineStep * scaleY;
    }
}

extern void CCache__BeginDisplayList(s32, s32, CFrameData *);
extern void Texture_InvalidateCache(void);
extern void Render_SetupFog(void);
extern void DLights_SwapBuffers(void);
extern void Controller_UpdatePakInsertion(void);
extern void CCameraPool__Advance(void *);
void CEngineApp__UpdateControllers(void);
void CEngineApp__DrawFilledRect(void *unused, u8 *color, s32 left, s32 top, s32 right, s32 bottom);
extern void Video_NextResolution(void);
extern s32 Automap_IsShown(void *);
extern void Automap_Draw(void *);
extern s32 CCache__EndDisplayList(s32);
extern void CEngineApp__SubmitGfxTask(CEngineApp *, CFrameData *);
extern void CCache__GetFrameDelay(void);
extern void Cinema_Update(void);

/* CEngineApp__DrawFade: darkens the whole screen by the level fade. */
static __inline__ void drawFade(CEngineApp *level) {
    u8 col[4];
    f32 t;
    s32 v;

    col[0] = 0;
    col[1] = 0;
    col[2] = 0;
    t = level->m_Settings.fade;
    if (t < 0.0f) goto opaque;
    if (t > 1.0f) {
        v = 0;
        goto done;
    }
    if (t < 0.0f) {
    opaque:
        v = 255;
    } else {
        v = ~(s32)(t * 255.0f);
    }
done:
    col[3] = v;
    CEngineApp__DrawFilledRect(level, col, 0, 0, gScreenWidth, gScreenHeight);
}

/* Draws one frame of the level and hands its display list to the RSP. */
void CEngineApp__Draw(CEngineApp *level) {
    u64 start;
    f32 inSpeed;
    f32 outSpeed;

    start = osGetTime();
    CCache__BeginDisplayList(0, gEvenOdd, gFrameData = gEngineApp.m_pCurrentFrameData);
    Texture_InvalidateCache();
    Render_SetupFog();
    ((void (*)(CEngineApp *)) CEngineApp__BeginDisplayList)(level);
    DLights_SwapBuffers();
    Controller_UpdatePakInsertion();
    CCameraPool__Advance(level->m_Game.m_CameraPool);
    ((void (*)(CEngineApp *)) CEngineApp__UpdateControllers)(level);
    if (gEngineModeTable[level->m_Settings.mode].draw != NULL) {
        gEngineModeTable[level->m_Settings.mode].draw(level);
    }
    if (gFrameData->m_bLines == 0) {
        drawFade(level);
    }
    if (gVideoClearFrames != 0) {
        if (gVideoClearFrames == 0x14) {
            Video_NextResolution();
        }
        clearDepth(level, 0, 0x1E0, 0, 0x168);
        {
            Gfx *g = gDList++;
            g->w0 = 0xED000000;     /* gDPSetScissor(0, 0, 479, 359) */
            g->w1 = 0x77C59C;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xFF1001DF;     /* gDPSetColorImage(frame, 480) */
            g->w1 = gFrameData->m_pFrameBuffer;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_FILL) */
            g->w1 = 0x00300000;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xFCFFFFFF;     /* gDPSetCombineMode(shade) */
            g->w1 = 0xFFFE793C;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE200001C;     /* gDPSetRenderMode(0, 0) */
            g->w1 = 0;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xF7000000;     /* gDPSetFillColor */
            g->w1 = 0x10001;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xF67805A0;     /* gDPFillRectangle(0, 0, 480, 360) */
            g->w1 = 0;
        }
        gVideoClearFrames--;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE9000000;         /* gDPFullSync */
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDF000000;         /* gSPEndDisplayList */
        g->w1 = 0;
    }
    gFrameData->m_DisplayListSize = (uptr)gDList - (uptr)gFrameData->m_pDisplayList;
    if (level->m_Settings.mode == 0xD) {
        gFrameData->m_bLines = Automap_IsShown(&level->m_Game.m_Automap);
    } else {
        gFrameData->m_bLines = 0;
    }
    if (gFrameData->m_bLines != 0) {
        gDList = gEngineApp.m_pCurrentFrameData->m_pLineList;
        Automap_Draw(&level->m_Game.m_Automap);
        drawFade(level);
        {
            Gfx *g = gDList++;
            g->w0 = 0xE9000000;     /* gDPFullSync */
            g->w1 = 0;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDF000000;     /* gSPEndDisplayList */
            g->w1 = 0;
        }
        gFrameData->m_LineListSize = (uptr)gDList - (uptr)gFrameData->m_pLineList;
    }
    gFrameData->m_nPredictTicks = gNextTicks;
    gRenderTime = (f32)(((osGetTime() - start) * 64) / 3) * 0.000001f;   /* OS_CYCLES_TO_USEC */
    if (CCache__EndDisplayList(0) != 0) {
        CEngineApp__SubmitGfxTask(level, gFrameData);
    } else {
        CCache__GetFrameDelay();
        osSendMesg(&gGfxFrameMessageQueue, &gFrameData->m_Msg, 1);
    }
    if (level->m_Settings.fadeMode != 0) {
        if (level->m_Settings.fastFade != 0) {
            inSpeed = 0.5f;
            outSpeed = 0.5f;
        } else {
            inSpeed = 0.1f;
            outSpeed = 0.1f;
        }
        switch (level->m_Settings.fadeMode) {
        case 2:
            if (level->m_Settings.fade > 0.0f) {
                level->m_Settings.fade -= outSpeed * gFrameIncrement;
                if (level->m_Settings.fade < 0.0f) {
                    level->m_Settings.fade = 0.0f;
                }
            } else {
                level->m_Settings.fade -= 1.0f;
            }
            if (level->m_Settings.fade < -4.0f) {
                s32 old = level->m_Settings.mode;
                level->m_Settings.mode = 0x11;
                level->m_Settings.fade = 0.0f;
                level->m_Settings.fadeMode = 1;
                level->m_Settings.prevMode = old;
            }
            break;
        case 1:
            level->m_Settings.fade += inSpeed * ((gFrameIncrement < 0.5f) ? 0.5f : gFrameIncrement);
            if (level->m_Settings.fade >= 1.0f) {
                level->m_Settings.fadeMode = 0;
                level->m_Settings.fade = 1.0f;
                level->m_Settings.fastFade = 0;
            }
            break;
        }
    }
    Cinema_Update();
}

extern s32 gInSinglePlayerMenu; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 gCameraPoolHideViews; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */
/* globals/menu.h's MenuPool read as bytes: this view (its type here) is what keeps the ROM's code */
extern u8 gMenuPoolBytes[] __asm__("gEngineApp_m_Game_m_MenuPool");
extern s32 MenuPool_CountOpen(void *);
extern s32 Demo_GetPlayerCount(void);
extern s32 Cinema_PathPlaying(void);
extern void Demo_AdvanceMatchStream(void);
extern void CCache__Advance(s32);
void CEngineApp__UpdatePlayTime(CEngineApp *arg0);
extern s32 Cinema_GetScriptId(void);
extern void Cinema_StopAtEnd(void);
extern s32 Cinema_IsFinished(void);
/* Retraces per frame: the mean of the last four frames without the slowest, at least `min`. */
#define SLOWEST ({ u32 _m = a; if (_m < d) _m = d; if (_m < c) _m = c; if (_m < b) _m = b; _m; })
#define MEAN ((sum - ({ u32 _t = SLOWEST; _t - 2; })) / 3)

static __inline__ u32 frameRetraces(u32 min) {
    u32 a = gNumTicks3;
    u32 b = gNumTicks0;
    u32 c = gNumTicks1;
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
        if ((u32) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount >= 3) {
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
    gFrameIncrement = (f32)(gNextTicks * 15) / gRefreshRate;
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
    if (MenuPool_CountOpen(gMenuPoolBytes) == 0 && Cinema_PathPlaying() != 0 && Cinema_GetScriptId() == 0x78) {
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
void CEngineApp__Boot(void);

void Engine_Boot(void) {
    ((void (*)(void *)) CEngineApp__Boot)(&gEngineApp);
}
void Engine_Idle(sptr arg0);

void CEngineApp__Boot(void) {
    memset(gThreadStackIdle, 0, sizeof(gThreadStackIdle));
    osCreateThread(&gThreadIdle, 0, Engine_Idle, NULL, gThreadStackIdle + sizeof(gThreadStackIdle), gThreadPriorityIdleInit);
    osStartThread(&gThreadIdle);
}
void mainproc(void);

void CEngineApp__Idle(CEngineApp *arg0, void *arg1) {
    memset(gThreadStackMain, 1, sizeof(gThreadStackMain));
    /* the stack top given is gSchedulerStack, which follows gThreadStackMain */
    osCreateThread(&gThreadMain, 1, mainproc, arg1, gSchedulerStack, gThreadPriorityMain);
    osStartThread(&gThreadMain);
    {
        s32 v = gThreadPriorityIdle;

        arg0->m_Settings.ready = 0;
        osSetThreadPri(0, v);
    }
loop_1:
    goto loop_1;
}

void CEngineApp__AdvanceFrameData(CEngineApp *gfx) {
    gFrameCount++;
    gFrameCountGameplay++;
    gEvenOdd ^= 1;
    gfx->m_pCurrentFrameData = &gfx->m_FrameData[gFrameCount % gTotalFramebuffers];
    gfx->m_pCurrentFrameData->m_pDisplayList = (void *)(sptr)(gEvenOdd ? gDisplayList0 : gDisplayList1);
    gfx->m_pCurrentFrameData->m_pLineList = (void *)(sptr)(gEvenOdd ? gLineList0 : gLineList1);
}

extern void Render_SetCombineMode(u32);

void CEngineApp__DrawFilledRect(void *unused, u8 *color, s32 left, s32 top, s32 right, s32 bottom) {
    if (color[3] != 0) {
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (20) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x0)) << 20))); };
        Render_SetRenderMode(0x13);
        Render_SetCombineMode(0x19);
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((((unsigned int)(color[0]) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(color[1]) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(color[2]) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(color[3]) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
        {
            Gfx *cmd = gDList++;
            s32 x, y, x0, y0;
            u32 high, low, high0;
            x = right;
            if (x < 0) x = 0;
            y = bottom;
            high = (((unsigned int)(x) & (0xFFFFFFFFU >> (32 - (10)))) << (14));
            if (y < 0) y = 0;
            low = (((unsigned int)(y) & (0xFFFFFFFFU >> (32 - (10)))) << (2)) | (((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24));
            cmd->w0 = high | low;
            x0 = left;
            if (x0 < 0) x0 = 0;
            y0 = top;
            high0 = (((unsigned int)(x0) & (0xFFFFFFFFU >> (32 - (10)))) << (14));
            if (y0 < 0) y0 = 0;
            cmd->w1 = high0 | (((unsigned int)(y0) & (0xFFFFFFFFU >> (32 - (10)))) << (2));
        }
    }
}

/* Fills the rectangle (x0, y0)-(x1, y1) of the z-buffer, then points the color image back at the frame. */
void CEngineApp__ClearDepthRect(sptr unused, s32 x0, s32 x1, s32 y0, s32 y1) {
    s32 colorImage;

    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    Render_SetRenderMode(0x14);
    colorImage = 0xFF100000 | ((gScreenWidth - 1) & 0xFFF);
    {
        Gfx *g = gDList++;
        g->w0 = colorImage;     /* gDPSetColorImage(RGBA, 16b, width, z-buffer) */
        g->w1 = gDepthBuffer;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_FILL) */
        g->w1 = 0x00300000;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xF7000000;     /* gDPSetFillColor(max depth) */
        g->w1 = 0xFFFCFFFC;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xF6000000 | ((x1 & 0x3FF) << 14) | ((y1 & 0x3FF) << 2); /* gDPFillRectangle */
        g->w1 = ((x0 & 0x3FF) << 14) | ((y0 & 0x3FF) << 2);
    }
    {
        Gfx *g = gDList++;
        g->w0 = colorImage;     /* gDPSetColorImage(RGBA, 16b, width, frame) */
        g->w1 = gEngineApp.m_pCurrentFrameData->m_pFrameBuffer;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_1CYCLE) */
        g->w1 = 0x00100000;
    }
}

void Demo_UpdateInput();                            /* extern */
void Controller_LatchAll();                            /* extern */

void CEngineApp__UpdateControllers(void) {
    Controller_LatchAll();
    Demo_UpdateInput();
}

/* The multiplayer match state at gEngineApp_m_Settings_match: the fields this resets. */

/* The game/menu context this runs on. */


void CEngineApp__StartGameAtWarp(CEngineApp *arg0, s32 arg1) {
    u8 sp18[0x18];
    s32 var_s0;

    var_s0 = arg1;
    if (CScene__FindWarpDests(&gScene, -1, var_s0, sp18, 1) == 0) {
        var_s0 = 0;
    }
    gEngineApp_m_Settings.match.frames = 0;
    gEngineApp_m_Settings.match.seconds = 0;
    gEngineApp_m_Settings.match.minutes = 0;
    gEngineApp_m_Settings.match.hours = 0;
    gEngineApp_m_Settings.match.unk44 = 0;
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.matchMode = var_s0;
    arg0->m_Settings.warpFlags = 1;
    arg0->m_Settings.nextMode = 0xD;
}

s32 Util_StrCmp(u8 *a, u8 *b) {
    if (a == ((void *)0) || b == ((void *)0)) {
        return -1;
    }
    while (*a != 0 && *b != 0) {
        if (*a < *b) {
            return -1;
        }
        if (*b < *a) {
            return 1;
        }
        a++;
        b++;
    }
    if (*a != 0) {
        return 1;
    }
    if (*b != 0) {
        return -1;
    }
    return 0;
}

/* strcpy */
u8 *Util_StrCpy(u8 *dst, u8 *src) {
    u8 *d = dst;

    while ((*d++ = *src++) != 0) {
    }
    return dst;
}

void Engine_Idle(sptr arg0) {
    CEngineApp__Idle(&gEngineApp, arg0);
}

s32 __osGetFpcCsr();                                /* extern */
void __osSetFpcCsr(s32);                         /* extern */
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);

void mainproc(void) {
#ifdef TARGET_PORT
    sprintf(gEngineTimestamp, "%s %s", "Nov 10 1998", "13:17:04");
#else
    sprintf(gEngineTimestamp, sEngineStampFmt, sEngineStampDate, sEngineStampTime);
#endif
    __osSetFpcCsr(__osGetFpcCsr() | 0x400);
    CEngineApp__Main(&gEngineApp);
}

#define APP_SCENE T2_OFFSETOF(CEngineApp, m_Scene)
#define APP_ONSCREEN T2_OFFSETOF(CEngineApp, unk19440)
#define APP_GAME T2_OFFSETOF(CEngineApp, m_Game)
#define APP_CAMERAPOOL T2_OFFSETOF(CEngineApp, m_Game.m_CameraPool)
#define APP_AUTOMAP T2_OFFSETOF(CEngineApp, m_Game.m_Automap)

void CEngineApp__NewGame(sptr arg0, s32 arg1, s32 arg2) {
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): a1 passes through to CCameraPool__NewGame on MIPS */
    CCameraPool__NewGame((void *) (arg0 + APP_CAMERAPOOL), arg1);

#else
    ((void (*)(void *)) CCameraPool__NewGame)((void *) (arg0 + APP_CAMERAPOOL)); /* a1 passes through */

#endif
    CPlayerPool__NewGame((void *) (arg0 + APP_GAME), arg1, arg2);
}

void CEngineApp__InitFade(CEngineApp *arg0) {
    arg0->m_Settings.fadeMode = 1;
    arg0->m_Settings.fade = 0;
    arg0->m_Settings.fastFade = 0;
}

void CEngineApp__UpdateFade(CEngineApp *g) {
    f32 speed;
    f32 kk;

    if (g->m_Settings.fadeMode != 0) {
        speed = 0.1f;
        if (g->m_Settings.fastFade != 0) {
            /* Redundant branches kept: they split the blocks so fadeState is reloaded. */
            if (g || gFrameIncrement) {
                speed = 0.5f;
            } else {
                speed = 0.5f;
            }
        }
        if (g) {
            kk = speed;
        } else {
            kk = speed;
        }
        if (g->m_Settings.fadeMode != 1) {
            if (g->m_Settings.fadeMode == 2) {
                if (0.0f < g->m_Settings.fade) {
                    g->m_Settings.fade = g->m_Settings.fade - (kk * gFrameIncrement);
                    if (g->m_Settings.fade < 0.0f) {
                        g->m_Settings.fade = 0.0f;
                    }
                } else {
                    g->m_Settings.fade = g->m_Settings.fade - 1.0f;
                }
                if (g->m_Settings.fade < -4.0f) {
                    s32 old = g->m_Settings.mode;
                    g->m_Settings.mode = 0x11;
                    g->m_Settings.fade = 0.0f;
                    g->m_Settings.fadeMode = 1;
                    g->m_Settings.prevMode = old;
                }
            }
        } else {
            g->m_Settings.fade = g->m_Settings.fade + (speed * ((gFrameIncrement < 0.5f) ? 0.5f : gFrameIncrement));
            if (1.0f <= g->m_Settings.fade) {
                g->m_Settings.fadeMode = 0;
                g->m_Settings.fade = 1.0f;
                g->m_Settings.fastFade = 0;
            }
        }
    }
}

void CEngineApp__DrawFade(CEngineApp *g) {
    u8 col[4];
    f32 t;
    s32 v;
    col[0] = 0;
    col[1] = 0;
    col[2] = 0;
    t = g->m_Settings.fade;
    if (t < 0.0f) goto c255;
    if (1.0f < t) { v = 0; goto done; }
    if (t < 0.0f) { c255: v = 255; } else { v = ~(s32)(t * 255.0f); }
    done:;
    col[3] = v;
    CEngineApp__DrawFilledRect(g, col, 0, 0, gScreenWidth, gScreenHeight);
}

s32 SavedPlayers_IsActive();                                /* extern */

/* The frame/game state block passed to the per-mode handlers (the
 * handler table gEngineModeTable has 0xC-byte rows indexed by mode). */

void CEngineApp__SetMode(CEngineApp *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = arg0->m_Settings.mode;
    arg0->m_Settings.fadeMode = 1;
    arg0->m_Settings.fade = 0.0f;
    arg0->m_Settings.mode = 0x11;
    arg0->m_Settings.nextMode = arg1;
    arg0->m_Settings.prevMode = temp_v1;
    if (SavedPlayers_IsActive() != 0) {
        arg0->m_Settings.fade = 1.0f;
    }
}

/* The game state (CEngineApp in CEngineApp__Construct). */

void CEngineApp__SetupFadeTo(CEngineApp *arg0, s32 arg1) {
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.nextMode = arg1;
}

s32 SavedPlayers_UpdateTimer(void);

/* The frame/game state block passed to the per-mode handlers (the
 * handler table gEngineModeTable has 0xC-byte rows indexed by mode). */

void CEngineApp__Retrace(CEngineApp *arg0) {
    void (*temp_v0)(struct CEngineApp *);

    if (gFirstFrame != 0) {
        gFirstFrame = 0;
    }
    SavedPlayers_UpdateTimer();
    temp_v0 = gEngineModeTable[arg0->m_Settings.mode].retrace;
    if (temp_v0 != NULL) {
        temp_v0(arg0);
    }
    if ((arg0->m_Settings.mode != 0x11) && (gDisplayListCount != 0)) {
        gDisplayListCount -= 1;
        CEngineApp__Update((sptr) arg0);
    }
    gRetraceCount += 1;
}

s32 CEngineApp__TimedModeSwitch(CEngineApp *ctx, f32 value, s32 normalNext, s32 forcedNext) {
    s32 *flag = &gMergedController.buttonsSingle;

    if (*flag != 0 && forcedNext != -1) {
        ctx->m_Settings.fadeMode = 2;
        ctx->m_Settings.nextMode = forcedNext;
        *flag = 0;
        return 1;
    }
    if (value * 15.0f < ctx->m_Settings.modeTimer) {
        ctx->m_Settings.fadeMode = 2;
        ctx->m_Settings.nextMode = normalNext;
        return 1;
    }
    return 0;
}

s32 Cinema_RequestById(s32);                         /* extern */

void CEngineApp__StartWarp(sptr arg0, s32 arg1) {
    CCameraPool__NewGame((void *) (arg0 + APP_CAMERAPOOL), 1);
    CPlayerPool__NewGame((void *) (arg0 + APP_GAME), 1, 0);
    CScene__RequestWarp(&gScene, ~Cinema_RequestById(arg1), 0);
    CScene__LoadBitSections(&gScene);
    CScene__LoadBitSections2(&gScene);
}

/* Turok 2: the mode block sits at 0x23FD0 (Rage Wars 0x26DB0). */

extern s32 Cinema_IsIdle(s32 *flags);

void CEngineApp__UpdateCinemaMode(CEngineApp *session, s32 arg1, s32 target) {
    s32 pending;
    s32 *flags;
    flags = &gMergedController.buttonsSingle;
    pending = *flags & 0x1000;
    if ((unsigned)(session->m_Settings.mode - 0xB) < 2) {
        pending = 0;
    }
    if (pending != 0 && target != -1) {
        session->m_Settings.fadeMode = 2;
        session->m_Settings.nextMode = target;
        *flags = 0;
        return;
    }
    if (Cinema_IsIdle(flags) != 0) {
        s32 mode = session->m_Settings.mode;
        session->m_Settings.fadeMode = 1;
        session->m_Settings.fade = 0;
        session->m_Settings.mode = 0x11;
        session->m_Settings.nextMode = arg1;
        session->m_Settings.prevMode = mode;
        if (SavedPlayers_IsActive() != 0) {
            session->m_Settings.fade = (1.0f);
        }
    }
}

s32 COnScreen__DrawBossBars(sptr);                         /* extern */
s32 func_00281354(sptr);                         /* extern */
s32 Automap_Update(sptr);                         /* extern */

void CEngineApp__DrawWorld(sptr arg0) {
    sptr temp_s0;

    temp_s0 = arg0 + APP_AUTOMAP;
    Automap_Update(temp_s0);
    CScene__Update(arg0 + APP_SCENE);
    func_00281354(arg0 + APP_CAMERAPOOL);
    COnScreen__DrawBossBars(arg0 + APP_ONSCREEN);
    if (Automap_IsShown((void *) temp_s0) == 0) {
        CEngineApp__DrawFade(arg0);
    }
}

/* A play-time clock: frames, seconds, minutes, hours. */

/* Only the field read here. */


void CEngineApp__UpdatePlayTime(CEngineApp *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f1;

    if (arg0->m_Settings.paused == 0) {
        temp_f1 = gEngineApp_m_Settings.match.frames + gFrameIncrement;
        gEngineApp_m_Settings.match.frames = temp_f1;
        if (temp_f1 >= 15.0f) {
            temp_f0 = gEngineApp_m_Settings.match.seconds + 1.0f;
            gEngineApp_m_Settings.match.frames = (f32) (temp_f1 - 15.0f);
            gEngineApp_m_Settings.match.seconds = temp_f0;
            if (6.0e1f <= temp_f0) {
                temp_f0_2 = gEngineApp_m_Settings.match.minutes + 1.0f;
                gEngineApp_m_Settings.match.seconds = (f32) (temp_f0 - 6.0e1f);
                gEngineApp_m_Settings.match.minutes = temp_f0_2;
                if (6.0e1f <= temp_f0_2) {
                    gEngineApp_m_Settings.match.minutes = (f32) (temp_f0_2 - 6.0e1f);
                    gEngineApp_m_Settings.match.hours = (f32) (gEngineApp_m_Settings.match.hours + 1.0f);
                }
            }
        }
    }
}

void EngineMode13_Game_Draw(sptr arg0) {
    sptr temp_s0;

    temp_s0 = arg0 + APP_AUTOMAP;
    Automap_Update(temp_s0);
    CScene__Update(arg0 + APP_SCENE);
    func_00281354(arg0 + APP_CAMERAPOOL);
    COnScreen__DrawBossBars(arg0 + APP_ONSCREEN);
    if (Automap_IsShown((void *) temp_s0) == 0) {
        CEngineApp__DrawFade(arg0);
    }
}

void func_00289108(void) {

}

extern s32 PakMenu_PickDefaultPort(void);
extern s32 PakMenu_HasPort(void);

void EngineMode00_PakCheck_Init(sptr arg0) {
    gPakMenuPort = -1;
    gPakCheckTimedOut = 0;
    CEngineApp__NewGame(arg0, 0, 0);
    CScene__LoadLevel(&gScene, 0, 0);
    PakMenu_PickDefaultPort();
    gPakCheckHasPak = PakMenu_HasPort();
    gPakCheckStarted = 1;
}

s32 Controller_UnwatchAllPaks();                            /* extern */

void EngineMode00_PakCheck_Retrace(CEngineApp *app) {
    if (gPakCheckStarted != 0) {
        if (gPakCheckTimer > 0.0f) {
            gPakCheckTimer -= gFrameIncrement;
        }
        if ((gPakCheckTimer <= 0.0f) && (gPakCheckTimedOut == 0)) {
            gPakCheckTimer = 0.0f;
            gPakCheckTimedOut = 1;
            MenuPool_OpenMenu(app->m_Game.m_MenuPool, &gPakSaveGameAnyPakMenuDesc, 0, &gMergedController, 0);
        }
        if (gPakCheckDone != 0) {
            CEngineApp__SetupFadeTo(app, 1);
            Controller_UnwatchAllPaks();
        }
    }
}

void EngineMode00_PakCheck_Draw(sptr arg0) {
    if (gPakCheckStarted != 0) {
        if (gPakCheckHasPak != 0) {
            Render_DrawTextLines(arg0, 1, sPakCheckText, gScreenWidth / 2, 0x5A, 0xA, 1.0f, 1.0f);
            return;
        }
        EngineMode13_Game_Draw(arg0);
    }
}

/* The ROM bases every store on gEngineApp_m_Settings_match (the match), the settings at
 * -0x1D8 from it. */
void EngineMode01_Legal_Init(sptr arg0) {
    GameMatch *match = &gEngineApp_m_Settings.match;
    GameSettings *settings;

    match->unk44 = 0;
    settings = (GameSettings *)((u8 *)match - 0x1D8); /* gEngineApp_m_Settings */
    settings->defaultSlot.m_bActive = 1;
    settings->multiplayer = 0;
    settings->playerCount = 1;
    CEngineApp__NewGame(arg0, 0, 0);
    CScene__LoadLevel(&gScene, 0, 0);
}

u32 GetMemSize();

void EngineMode01_Legal_Retrace(CEngineApp *g) {
    s32 mode;
    u32 r = GetMemSize();
    mode = 4;
    if (r > 0x400000) {
        if (gShowExpansionPakScreen != 0) {
            mode = 3;
        }
    }
    if (g->m_Settings.modeTimer > 60.0f) {
        gLegalScreenNoSkip = 0;
    }
    if (gLegalScreenNoSkip != 0) {
        if (CEngineApp__TimedModeSwitch(g, 12.0f, mode, -1)) {
            gLegalScreenNoSkip = 0;
        }
    } else {
        if (CEngineApp__TimedModeSwitch(g, 12.0f, mode, mode)) {
            gLegalScreenNoSkip = 0;
        }
    }
}

void EngineMode01_Legal_Draw(sptr arg0) {
    f32 x;

    if (gScreenWidth >= 0x11D) {
        x = gScreenWidth / 2 - 12.0f;
    } else {
        x = gScreenWidth / 2;
    }
    Render_DrawTextLines(arg0, 1, sLegalText, x, 12, 10, 0.7f, 1.0f);
}

void EngineMode03_ExpansionPak_Init(sptr arg0) {
    CEngineApp__NewGame(arg0, 0, 0);
}

void EngineMode03_ExpansionPak_Retrace(sptr arg0) {
    if (CEngineApp__TimedModeSwitch(arg0, 4.0f, 4, 4) != 0) {
        gShowExpansionPakScreen = 0;
    }
}

void EngineMode02_DemoWarning_Init(void) {

}

void EngineMode02_DemoWarning_Retrace(sptr arg0) {
    CEngineApp__TimedModeSwitch(arg0, 14.0f, 5, 5);
}

void EngineMode02_DemoWarning_Draw(void *arg0) {
    Render_DrawTextLines(arg0, 1, sDemoWarningText, gScreenWidth / 2, 5, 0xA, 0.75f, 1.0f);
}

void EngineMode04_Cinema_Init(sptr arg0) {
    CEngineApp__StartWarp(arg0, 0x64);
}

void EngineMode04_Cinema_Retrace(sptr arg0) {
    CEngineApp__UpdateCinemaMode(arg0, 5, 5);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode04_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode04_Cinema_Draw(sptr pt0) {
    gFogEnabled = 0;
    CEngineApp__DrawWorld(pt0);
}

void EngineMode05_Cinema_Init(sptr arg0) {
    CEngineApp__StartWarp(arg0, 0x5A);
}

void EngineMode05_Cinema_Retrace(sptr arg0) {
    CEngineApp__UpdateCinemaMode(arg0, 7, 7);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode05_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode05_Cinema_Draw(sptr arg0) {
    CEngineApp__DrawWorld(arg0);
    ((void (*)(sptr)) func_00289108)(arg0);
}

void EngineMode06_Init(void) {

}

void EngineMode06_Retrace(void) {

}

void EngineMode06_Draw(void) {

}

extern void Music_SetTrack(s32);

void EngineMode07_Title_Init(void) {
    Music_SetTrack(0x10);
}

extern s32 Demo_Enable(void);

void EngineMode07_Title_Retrace(sptr arg0) {
    Demo_Enable();
    CEngineApp__TimedModeSwitch(arg0, 20.0f, 0xD, 8);
}

s32 Demo_End();                            /* extern */

void EngineMode08_MainMenu_Init(CEngineApp *app) {
    Demo_End();
    CEngineApp__NewGame(app, 0, 0);
    Music_SetTrack(0x10);
    CScene__LoadLevel(&gScene, 0, 0);
    MenuPool_OpenMenu(app->m_Game.m_MenuPool, &gTitleMenuDesc, 0, &gMergedController, 0);
}

void EngineMode08_MainMenu_Retrace(CEngineApp *arg0) {
    if (gMergedController.buttons != 0) {
        arg0->m_Settings.modeTimer = 0.0f;
    }
    if (arg0->m_Settings.modeTimer > 4.5e2f) {
        CEngineApp__SetupFadeTo((sptr) arg0, 7);
    }
}

/* The app arrives in a0 and goes straight on (the host needs it passed). */
void EngineMode08_MainMenu_Draw(sptr arg0) {
    EngineMode13_Game_Draw(arg0);
}

void EngineMode09_Cinema_Init(sptr arg0) {
    gAttractRunCount += 1;
    CEngineApp__StartWarp(arg0, 0x6F);
}

void EngineMode09_Cinema_Retrace(sptr arg0) {
    CEngineApp__UpdateCinemaMode(arg0, 7, 8);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode09_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode09_Cinema_Draw(sptr pt0) {
    CEngineApp__DrawWorld(pt0);
}

void EngineMode11_Cinema_Init(sptr arg0) {
    CEngineApp__StartWarp(arg0, 0x6F);
}

void EngineMode11_Cinema_Retrace(sptr arg0) {
    CEngineApp__UpdateCinemaMode(arg0, 0xC, 0xC);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode11_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode11_Cinema_Draw(sptr pt0) {
    CEngineApp__DrawWorld(pt0);
}

void EngineMode10_Cinema_Init(sptr arg0) {
    CEngineApp__StartWarp(arg0, 0x6F);
}

s32 Demo_Enable();                            /* extern */

void EngineMode10_Cinema_Retrace(sptr arg0) {
    Demo_Enable();
    CEngineApp__UpdateCinemaMode(arg0, 0xD, 8);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode10_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode10_Cinema_Draw(sptr pt0) {
    CEngineApp__DrawWorld(pt0);
}

void EngineMode12_Cinema_Init(sptr arg0) {
    Demo_End();
    CEngineApp__StartWarp(arg0, 0x8D);
}

void EngineMode12_Cinema_Retrace(sptr arg0) {
    CEngineApp__UpdateCinemaMode(arg0, 0xD, 0xD);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode12_Cinema_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode12_Cinema_Draw(sptr pt0) {
    CEngineApp__DrawWorld(pt0);
}

extern void Demo_PlayNext(void);
extern s32 SavedPlayers_GetWarpID(void);
extern void SavedPlayers_Restore(void);

void EngineMode13_Game_Init(CEngineApp *state) {
    s32 flag;

    gEngineApp_m_Settings.match.unk44 = 0;
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
    CEngineApp__NewGame(state, state->m_Settings.playerCount, flag);
    if (SavedPlayers_IsActive()) {
        SavedPlayers_Restore();
    }
    CCollisionInfo__Reset();
    gEmitterTimeScale = 1.0f;
    gEnemyTimeScale = 1.0f;
    gParticleTimeScale = 1.0f;
    Automap_ClearBuffers(&state->m_Game.m_Automap);
    ((void (*)(void *, s32)) CScene__InitPersistentData)(&gScene, state->m_CartridgeIndex); /* a1 unused */
    CScene__RequestWarp(&gScene, state->m_Settings.matchMode, 0);
    gInSinglePlayerMenu = 0;
}

void EngineMode13_Game_Retrace(void) {

}

void EngineMode14_Init(void) {

}

void EngineMode14_Retrace(void) {

}

void EngineMode14_Draw(void) {

}

#ifdef TARGET_PORT
#include "structs/player.h"
/* the view at the host offset of CPlayer m_Data.m_bPrimagenBossComplete (N64 0x9F0): the N64
 * offset read other player data on the host, so the credits after the Primagen never knew he was
 * beaten (no cinema 0x82, Credits_Update waited for good; round 11 z2) */
typedef struct {
    union {
        struct { u8 _hp_unk9F0[__builtin_offsetof(CPlayer, m_Data.m_bPrimagenBossComplete)]; s32 unk9F0; };
    };
} Unk002532A8;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x9F0];
    /* 0x9F0 */ s32 unk9F0;
} Unk002532A8;
#endif

void EngineMode15_Ending_Init(sptr arg0) {
    void *temp_v0;

    gFinalEnding = 0;
    sCreditsLine = 0;
    gCreditsScroll = (f32) gScreenHeight;
    gCreditsDone = 0;
    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if ((temp_v0 != NULL) && (((Unk002532A8 *)temp_v0)->unk9F0 != 0)) {
        gEndingPrimagenDefeated = 1;
    } else {
        gEndingPrimagenDefeated = 0;
    }
    if (gEndingPrimagenDefeated != 0) {
        CScene__RequestWarp(&gScene, ~Cinema_RequestById(0x82), 0);
        return;
    }
    CEngineApp__StartWarp(arg0, 0x82);
}

/* one-element array views of scalars (globals/controller.h, globals/menu.h), indexed by a zero variable for the match */
extern s32 gButtonsSingleView[] __asm__("gMergedController_buttonsSingle");
extern s32 gCreditsDoneView[] __asm__("gCreditsDone");
extern f32 gCreditsTimeLeftView[] __asm__("gCreditsTimeLeft");

void EngineMode15_Ending_Retrace(sptr arg0)
{
  int new_var;
  new_var = 0;
  if (gFinalEnding != 0)
  {
    CEngineApp__UpdateCinemaMode(arg0, 1, 1);
    return;
  }
  if ((gButtonsSingleView[0] & 0x1000) && (gCreditsDoneView[new_var] == 0))
  {
    gCreditsDoneView[new_var] = 1;
    gButtonsSingleView[new_var] = 0;
    gCreditsTimeLeftView[new_var] = 15.0f;
  }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode15_Ending_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode15_Ending_Draw(sptr arg0) {
    CEngineApp__DrawWorld(arg0);
    ((void (*)(sptr)) Credits_Draw)(arg0);
    Credits_Update(arg0);
}

#define GAME_SETTINGS_OFF T2_OFFSETOF(CGame, m_Settings)

void EngineMode16_Ending_Init(sptr arg0)
{
  CPlayer *temp_v0;
  s8 *new_var;
  new_var = (s8 *) (&gEngineApp_m_Settings);
  gEngineApp_m_Settings.multiplayer = 0;
  gFinalEnding = 0;
  sCreditsLine = 0;
  gCreditsScroll = (f32) gScreenHeight;
  gCreditsDone = 0;
  temp_v0 = CPlayerPool__GetFirstActive(new_var - GAME_SETTINGS_OFF);        /* the game state, gEngineApp_m_Game */
  if ((temp_v0 != 0) && (temp_v0->m_Data.m_bPrimagenBossComplete != 0))
  {
    gEndingPrimagenDefeated = 1;
  }
  else
  {
    gEndingPrimagenDefeated = 0;
  }
  if (gEndingPrimagenDefeated != 0)
  {
    CScene__RequestWarp(&gScene, ~Cinema_RequestById(0x82), 0);
  }
  else
  {
    CEngineApp__StartWarp(arg0, 0x82);
  }
  gFinalEnding = 1;
  gEndingPrimagenDefeated = 0;
}

void EngineMode16_Ending_Retrace(sptr arg0) {
    if (gFinalEnding != 0) {
        CEngineApp__UpdateCinemaMode(arg0, 1, 1);
        return;
    }
    if ((gMergedController_buttonsSingle & 0x1000) && (gCreditsDone == 0)) {
        gCreditsDone = 1;
        gMergedController_buttonsSingle = 0;
        gCreditsTimeLeft = 15.0f;
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CEngineApp__DrawWorld receives EngineMode16_Ending_Draw's own argument
 * registers (pass-through); the host passes them. */
void EngineMode16_Ending_Draw(sptr arg0) {
    CEngineApp__DrawWorld(arg0);
    ((void (*)(sptr)) Credits_Draw)(arg0);
    Credits_Update(arg0);
}

/* The game state (CEngineApp in CEngineApp__Construct). */

s32 osViSetSpecialFeatures(s32);
s32 osViSetYScale(f32);                         /* extern */

void EngineMode17_Switch_Init(CEngineApp *arg0) {
    OSViMode *var_a0;

    if (arg0->m_Settings.unk1C8 != 0) {
        var_a0 = &osViModeNtscHpn1;
        if (gResolution == 0) {
            var_a0 = &osViModeTable[0];
        }
        osViSetMode(var_a0);
        osViSetYScale(1.0f);
        osViSetSpecialFeatures(2);
    }
}

s32 Cinema_Stop(void);
s32 Demo_End(void);
s32 Sound__StopAllExcept(s32);                     /* extern */
s32 Music_FadeOut();                            /* extern */

/* The frame/game state block passed to the per-mode handlers (the
 * handler table gEngineModeTable has 0xC-byte rows indexed by mode). */

void EngineMode17_Switch_Retrace(CEngineApp *arg0) {
    void (*temp_v0)(struct CEngineApp *);
    s32 temp_v1;

    if ((gDisplayListCount == 2) && (arg0->m_Settings.unk1C8 == 0)) {
        arg0->m_Settings.modeTimer = 0;
        arg0->m_Settings.mode = arg0->m_Settings.nextMode;
        Cinema_Stop();
        Sound__StopAllExcept(-1);
        temp_v1 = arg0->m_Settings.nextMode;
        if ((temp_v1 != 8) && (temp_v1 != 0xC)) {
            if ((arg0->m_Settings.prevMode != 8) || (arg0->m_Settings.mode != 0xD)) {
                Music_SetTrack(-1);
                Music_FadeOut();
                goto block_7;
            }
        } else {
block_7:
            if (arg0->m_Settings.mode != 0xD) {
                Demo_End();
            }
        }
        Controller_SetRumbleOptionAll(0);
        temp_v0 = gEngineModeTable[arg0->m_Settings.mode].init;
        if (temp_v0 != NULL) {
            temp_v0(arg0);
        }
    }
}

void EngineMode17_Switch_Draw(void) {

}

/* Turok 2 layouts (Rage Wars offsets in parentheses). */
typedef struct B21GameBlk {
    u8 pad0[0x44];
    s32 unk44;          /* 0x44 (0x88) */
} B21GameBlk;
s32 MenuPool_CountOpen(void *a);
s32 Cinema_PathPlaying();
s32 Cinema_GetScriptId();
s32 Cinema_IsFinished();
void Cinema_StopAtEnd();

s32 CEngineApp__CheckEndCinema(CEngineApp *g) {
    B21GameBlk *b;
    s32 r;
    if (MenuPool_CountOpen(gEngineApp_m_Game.m_MenuPool) == 0) {
        if (Cinema_PathPlaying()) {
            if (Cinema_GetScriptId() == 0x78) {
                b = (B21GameBlk *) &gEngineApp_m_Game.m_Settings.match;
                if (b->unk44 != 0) {
                    goto check;
                }
                if (g->m_Settings.paused == 0) {
                    b->unk44 = 1;
                    Cinema_StopAtEnd();
                }
            }
        }
    }
    if (gEngineApp_m_Settings.match.unk44 != 0) {
check:
        r = Cinema_IsFinished();
        if (r) return 1;
    }
    return 0;
}

s32 CPlayerPool__StopRumble(CPlayerPool *);                   /* extern */
s32 RumblePool_FreeAll(RumblePool *pool);                   /* extern */

void CEngineApp__StopRumble(void) {
    RumblePool_FreeAll(&gRumblePool);
    CPlayerPool__StopRumble(&gEngineApp_m_Game.m_PlayerList);
}
