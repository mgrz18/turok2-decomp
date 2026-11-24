/* MATCHES at its real start (checked with the symbol moved to 0x42EB50, size 0xC0). */
/* WRONG CUT: the real function starts at 0x00467B50 (size 0xC0 = 192); the word
 * at 0x00467B4C (lw v1,0x20(sp)) is junk between objects. */
#include "common.h"
#include "structs/demo.h"
/* Second VM module copy of Demo_DecodeButtons (same source, linked 0xE0 lower). */
#include "common.h"
/* Second VM module copy of Demo_Start (linked 0x39000 lower; here the cut is whole, 0x298). The settings loop is written out in the function (not an inline) so both loops share the counter i ($a2), as in the ROM. */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/engine.h"
/* Second VM module copy of Demo_Update. */
#include "common.h"
#include "structs/controller.h"
/* Second VM module copy of Demo_UpdateStreams (same source, linked 0xE0 lower); the per-timer calls are inline helpers, which keeps GCC from strength-reducing &s->buttons[i]. */
#include "common.h"
#include "common.h"
/* The second VM module's copy of src/us/vm1/Demo/func_0042F514.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "m2c_macros.h"
#include "globals/demo.h"
/* The second VM module's copy of src/us/vm1/Demo/func_0042F600.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/Demo/func_0042F6A4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/Demo/func_0042F6C8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/Demo/func_0042F700.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* Second VM module copy of Demo_DecodeStick (same source, linked 0xE0 lower). */
#include "common.h"
/* Second VM module copy of Demo_DecodeMatch (same source, linked 0xE0 lower). */
#include "common.h"
/* The second VM module's copy of src/us/vm1/Demo/func_0042F868.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/Demo/func_0042F958.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "globals/frame.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A170_de, with Turok 2's layouts. */
/* Second VM module copy of CCamera__Construct (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/camera.h"
#include "globals/render.h"
#include "protos/list.h"
#include "structs/twin_demo.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A370_de, with Turok 2's layouts. */
/* Second VM module copy of CCameraPool__NewGame (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/emitter.h"
/* Bit stream: read a length code (3 bits giving a bit count n, then n bits). */

#define READ_BITS(br, n, value)                                              \
    {                                                                        \
        s32 _n = (n);                                                        \
        u32 _pos;                                                            \
        value = 0;                                                           \
        while (--_n != -1) {                                                  \
            _pos = (br)->pos++;                                              \
            value <<= 1;                                                     \
            value |= ((br)->bits[_pos >> 3] & (0x80 >> (_pos & 7))) != 0;   \
        }                                                                    \
    }

void Demo_ReadRunLength_2(DemoStream *br) {
    s32 n;
    s32 bits;

    READ_BITS(br, 3, n);
    if (n != 0) {
        READ_BITS(br, n, bits);
        br->frames = bits + 1;
    } else {
        br->frames = 1;
    }
}

static __inline__ s32 readBit(DemoStream *r) {
    u32 pos = r->pos++;

    return (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
}

/* Reads a nine-bit code, its bits stored in a scrambled order, then decodes it. */
void Demo_DecodeButtons_2(DemoStream *r) {
    r->value = readBit(r) << 15;
    r->value |= readBit(r) << 14;
    r->value |= readBit(r) << 4;
    r->value |= readBit(r) << 13;
    r->value |= readBit(r) << 3;
    r->value |= readBit(r) << 1;
    r->value |= readBit(r);
    r->value |= readBit(r) << 2;
    r->value |= readBit(r) << 8;
    Demo_ReadRunLength_2(r);
}
void Demo_DecodeStick_2(DemoStream *t);
void Demo_DecodeMatch_2(DemoStream *t);
extern void GameSettings_SetDefaults_2(GameSettings *setup);
extern s32 Random_Next(void);

static __inline__ void initSetting(DemoStream *t, DemoHeader *config, s32 index, void (*handler)(), s32 value) {
    t->bits = (u8 *)config;
    t->player = index;
    t->frames = 1;
    t->decode = handler;
    t->unk18 = 0;
    t->unk1C = 0;
    t->pos = value;
}

/* Starts a match from config: sets up the per-player settings and the game setup. */
void Demo_Start_2(DemoHeader *arg) {
    DemoPlayer *s = &gDemoPlayer;
    GameSettings *setup;
    s32 i;
    s32 r;

    DemoHeader *owner = arg;
    DemoHeader *config = arg;

    gAttractRunCount++;
    s->unk4 = 0;
    s->playing = 1;
    s->done = 0;
    s->fastForward = 1;
    if (s->saved == 0) {
        s->saved = 1;
        s->settings = *(GameSettingsBlock *)&gEngineApp_m_Settings;
    }
    {
        GameSettings *g = &gEngineApp_m_Settings;
        for (i = 0; i < 4; i++) {
            g->defaultSlot.m_bActive = 0;
            g->slots[i].m_bActive = 0;
            initSetting(&s->buttons[i], owner, i, Demo_DecodeButtons_2, config->buttonsStart[i]);
            initSetting(&s->stickX[i], owner, i, Demo_DecodeStick_2, config->stickXStart[i]);
            initSetting(&s->stickY[i], owner, i, Demo_DecodeStick_2, config->stickYStart[i]);
        }
    }
    initSetting(&s->match, owner, 0, Demo_DecodeMatch_2, config->matchStart);
    s->playerCount = config->playerCount;
    s->unk1B8 = 0;
    s->countdown = config->frames;
    s->time = 0;
    s->checksum = 0;
    gEngineApp_m_Settings.matchMode = config->matchMode;
    setup = &gEngineApp_m_Settings;
    GameSettings_SetDefaults_2(setup);
    if (s->playerCount >= 2) {
        for (i = 0; i < s->playerCount; i++) {
            setup->slots[i].m_bActive = 1;
            setup->slots[i].m_nCharacter = config->characters[i];
        }
        gEngineApp_m_Settings.multiplayer = 1;
    } else {
        setup->defaultSlot.m_bActive = 1;
        setup->multiplayer = 0;
    }
    setup = &gEngineApp_m_Settings;
    setup->unk17 = 0;
    setup->playerCount = s->playerCount;
    setup->cheatFlags = 12;
    r = Random_Next() % 9;
    setup->dmTextureSet = r;
    setup->tagTextureSet = r;
    setup->teamTextureSet = r;
}

void CEngineApp__QuitToLegal_2(void *);
void CEngineApp__SetupFadeTo(void *, s32);

/* Demo_UpdateChecksum (here inlined) */
static __inline__ void updateChecksum(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += gFrameCountGameplay * s->buttons[i].value * 0x12345678;
        s->checksum += gFrameCountGameplay * s->stickX[i].value * 0x87654321;
        s->checksum += gFrameCountGameplay * s->stickY[i].value * 0x12348765;
    }
    s->checksum += gFrameCountGameplay * s->match.value * 0x87651234;
}

void Demo_Update_2(void) {
    DemoPlayer *s = &gDemoPlayer;

    s->fastForward = 0;
    s->time += gFrameIncrement;
    if (s->done == 0) {
        if (--s->countdown <= 0 || gMergedController.buttonsSingle != 0) {
            s->done = 1;
            CEngineApp__QuitToLegal_2(&gEngineApp);
            if (gMergedController.buttonsSingle != 0) {
                CEngineApp__SetupFadeTo(&gEngineApp, 8);
            }
        }
    }
    updateChecksum();
}

/* One per player, 0x224 bytes apart. */
typedef struct Slot {
    /* 0x00 */ s32 a;
    /* 0x04 */ u8 pad4[0x12];
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 c;
    /* 0x18 */ u8 pad18[0x224 - 0x18];
} Slot;
extern Slot gControllers_buttons[]; /* local type: globals/controller.h declares it; this view (its type here) is what keeps the ROM's code */

static __inline__ void fire(DemoStream *t) { t->decode(t); }

static __inline__ void tick(DemoStream *t) { if (--t->frames <= 0) t->decode(t); }

void Demo_UpdateStreams_2(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    if (s->fastForward != 0) {
        for (i = 0; i < s->playerCount; i++) {
            fire(&s->buttons[i]);
            fire(&s->stickX[i]);
            fire(&s->stickY[i]);
        }
    } else {
        for (i = 0; i < s->playerCount; i++) {
            tick(&s->buttons[i]);
            tick(&s->stickX[i]);
            tick(&s->stickY[i]);
        }
    }
    for (i = 0; i < s->playerCount; i++) {
        if (s->done != 0) {
            gControllers_buttons[i].a = 0;
            gControllers_buttons[i].b = 0;
            gControllers_buttons[i].c = 0;
        } else {
            gControllers_buttons[i].a = s->buttons[i].value;
            gControllers_buttons[i].b = s->stickX[i].value;
            gControllers_buttons[i].c = s->stickY[i].value;
        }
    }
    Demo_Update_2();
}

void Demo_Reset_2(void) {
    gDemoPlayer.active = 0;
    gDemoPlayer.unk4 = 0;
    gDemoPlayer.playing = 0;
    gDemoPlayer.fastForward = 0;
    gDemoPlayer.index = 0;
    gDemoPlayer.saved = 0;
}

/* Ends a demo: puts back the game settings saved when it started. */
void func_00468434(void) {
    DemoPlayer *s = &gDemoPlayer;

    if (s->saved != 0) {
        s->saved = 0;
        *(GameSettingsBlock *)&gEngineApp_m_Settings = s->settings;
    }
    s->active = 0;
    s->unk4 = 0;
    s->playing = 0;
}

void Demo_Enable_2(void) {
    gDemoPlayer.active = 1;
}

void Demo_PlayNext_2(void) {
    DemoPlayer *s = &gDemoPlayer;

    if (s->active) {
        Demo_Start_2(gDemoTable_2[s->index]);
        s->index++;
        if (gDemoTable_2[s->index] == NULL) {
            s->index = 0;
        }
    }
}

void func_00468520(DemoStream *t, sptr bits, s32 pos, s32 player, void (*decode)(DemoStream *), s32 unk18, s32 unk1C) {
    t->bits = (u8 *)bits;
    t->pos = pos;
    t->player = player;
    t->frames = 1;
    t->decode = decode;
    t->unk18 = unk18;
    t->unk1C = unk1C;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 (*callback)();
} Arg0;

void DemoStream_Decode_2(Arg0 *arg0) {
    arg0->callback();
}

typedef struct {
    /* 0x00 */ s32 result;
    /* 0x04 */ u8 pad4[0xC - 0x4];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ s32 (*func)();
} Job;

void DemoStream_CallHandler2_2(Job *arg0) {
    arg0->func();
}

/* Only what this touches: a result, a word cleared and the callback. */
typedef struct {
    /* 0x00 */ s32 result;
    /* 0x04 */ u8 pad4[0xC - 0x4];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ s32 (*callback)();
} Task;

void DemoStream_RunHandler2_2(Task *arg0) {
    arg0->result = arg0->callback();
    arg0->unkC = 0;
}

/* Bit `bit` of a big-endian bit array. */
s32 func_004685C4(u8 *bits, u32 bit) {
    u8 *p = bits + (bit >> 3);
    s32 mask = 0x80 >> (bit & 7);

    return (*p & mask) != 0;
}

/* Reads the next bit. */
s32 func_004685E8(DemoStream *r) {
    u32 bit = r->pos++;

    return (r->bits[bit >> 3] & (0x80 >> (bit & 7))) != 0;
}

/* A timer whose durations are read from a bit stream (see Demo_ReadRunLength_2). */

/* Reads count bits, most significant first. */
s32 func_00468620(DemoStream *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->pos++;
        v |= (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

/* Reads count bits, most significant first. */
static __inline__ s32 readBits(DemoStream *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->pos++;
        v |= (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

void Demo_DecodeStick_2(DemoStream *t) {
    t->value = readBits(t, 8);
    Demo_ReadRunLength_2(t);
}

void Demo_DecodeMatch_2(DemoStream *t) {
    t->value = readBits(t, 2) + 2;
    Demo_ReadRunLength_2(t);
}

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 counter;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 (*callback)();
} Timer;

/* Counts down; calls the callback when it runs out. */
void DemoStream_Tick_2(Timer *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->counter - 1;
    arg0->counter = temp_v0;
    if (temp_v0 <= 0) {
        arg0->callback();
    }
}

/* Folds the timers' values into the state's checksum. */
void func_00468788(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += gFrameCountGameplay * s->buttons[i].value * 0x12345678;
        s->checksum += gFrameCountGameplay * s->stickX[i].value * 0x87654321;
        s->checksum += gFrameCountGameplay * s->stickY[i].value * 0x12348765;
    }
    s->checksum += gFrameCountGameplay * s->match.value * 0x87651234;
}

void func_00468878(void) {
    DemoPlayer *s = &gDemoPlayer;
    DemoStream *t = &s->match;

    if (s->fastForward != 0 || --t->frames <= 0) {
        t->decode(t);
    }
    gNextTicks = s->match.value;
}

typedef struct IntPair {
    s32 field_0;
    s32 field_4;
} IntPair;
extern IntPair gCameraVisBits; /* local type: one load pair over gCameraVisBits and gScreenClearTime */
void CCameraViewParams__Construct(void *);
void CMtxF__Identity(void *);
void CCamera__ResetWarp(CCamera *);
void CCamera__SetMode(CCamera *, s32);
void MenuPool_Construct_2(void *);
void CEmitterPool__Construct_2(CEmitterPool *arg0);
void CCamera__ConstructSkySystem(CCamera *);

void CCamera__Construct_2(CCamera *view) {
    f32 one;
    f32 zero;
    s16 size;
    s32 i;
    Floats *f1;
    Floats *f2;
    Floats *f3;
    Floats *f4;
    Floats *f5;

    CCameraViewParams__Construct(&view->m_ViewParams);
    CMtxF__Identity(view->m_mfViewOrient);
    CMtxF__Identity(view->m_mfProjection);
    zero = 0.0f;
    one = 1.0f;
    f1 = (Floats *)&view->m_qViewAlign;
    f1->unk8 = zero;
    f1->unk4 = zero;
    view->m_qViewAlign.x = zero;
    view->m_qViewAlign.w = one;
    CMtxF__Identity(view->m_mfInverseViewOrient);
    view->m_Viewport.m_Width = (f32)gScreenWidth;
    view->m_Viewport.m_Height = (f32)gScreenHeight;
    view->m_nCurrentRegionSet = -1;
    view->m_Viewport.m_ScreenPosX = zero;
    view->m_Viewport.m_ScreenPosY = zero;
    view->m_FogColor[0] = 0;
    view->m_LastFogColor[0] = 0;
    view->m_FogColor[1] = 0;
    view->m_LastFogColor[1] = 0;
    view->m_FogColor[2] = 0;
    view->m_LastFogColor[2] = 0;
    view->m_FogColor[3] = 0;
    view->m_LastFogColor[3] = 0;
    view->m_cRegionSetBlend = one;
    view->m_FogStart = 0x3E3;
    view->m_LastFogStart = 0x3E3;
    view->m_Flash = 0;
    view->m_PixelXScale = one;
    view->m_PixelYScale = one;
    view->m_FarClip = 1024.0f;
    view->m_LastFarClip = 1024.0f;
    view->m_FieldOfView = 47.5f;
    view->m_LastFieldOfView = 47.5f;
    CCamera__ResetWarp(view);
    f2 = (Floats *)&view->m_Shake;
    view->m_TremorAngle = zero;
    view->m_TremorHeight = zero;
    f2->unk8 = zero;
    f2->unkC = zero;
    f2->unk10 = zero;
    f2->unk14 = one;
    f3 = (Floats *)&view->m_Shake.m_XAxis;
    f3->unk4 = zero;
    f3->unk8 = zero;
    f3->unkC = zero;
    f3->unk10 = zero;
    f4 = (Floats *)&view->m_Shake.m_YAxis;
    f4->unk4 = zero;
    f4->unk8 = zero;
    f4->unkC = zero;
    f4->unk10 = zero;
    f5 = (Floats *)&view->m_Shake.m_ZAxis;
    f5->unk4 = zero;
    f5->unk8 = zero;
    f5->unkC = zero;
    f5->unk10 = zero;
    CCamera__SetMode(view, 0);
    size = gScreenWidth * 2;
    for (i = 0; i < 2; i++) {
        view->m_vp[i].vscale[0] = size;
        view->m_vp[i].vscale[1] = size;
        view->m_vp[i].vscale[2] = 0x3FF;
        view->m_vp[i].vscale[3] = 0;
        view->m_vp[i].vtrans[0] = size;
        view->m_vp[i].vtrans[1] = size;
        view->m_vp[i].vtrans[2] = 0;
        view->m_vp[i].vtrans[3] = 0;
    }
    MenuPool_Construct_2(view->m_MenuPool);
    CEmitterPool__Construct_2(view->m_SkySystem);
    CMtxF__Identity(view->m_mfFaceScreen);
    CMtxF__Identity(view->m_mfFaceScreenYAxisUp);
    view->m_FlashMode = 0;
    gCameraVisBits.field_4 = 0;
    CList__Construct(&view->m_ActiveTextList, 0, sizeof(void *));
    CCamera__ConstructSkySystem(view);
}

void Sound__SetListener(void *);
void CCache__Flush(s32);
void CCache__Deallocate(sptr, void *);
struct CMemEntry *CCache__Allocate(s32, s32, s32, char *);
void *memset(void *destination, s32 value, uptr size);

static __inline__ void clearAxis(CShakeAxis *axis) {
    axis->m_Shake = 0;
    axis->m_LastShake = 0;
    axis->m_Amp = 0;
    axis->m_Angle = 0;
}

static __inline__ void initShake(CShake *shake, f32 radius) {
    shake->m_vPos.x = 0;
    shake->m_vPos.y = 0;
    shake->m_vPos.z = 0;
    shake->m_Radius = radius;
    clearAxis(&shake->m_XAxis);
    clearAxis(&shake->m_YAxis);
    clearAxis(&shake->m_ZAxis);
}

void CCameraPool__NewGame_2(CCameraPool *owner, s32 count) {
    s32 i;
    f32 value;

    owner->m_ActiveLastFrame = 0;
    owner->m_bActiveChanged = 0;
    owner->m_MainCamera.m_Index = 0;
    CCamera__Construct_2(&owner->m_MainCamera);
    owner->m_bActiveChanged = 2;
    CList__Construct(&owner->m_ShakeFreeList, 0, sizeof(void *));
    CList__Construct(&owner->m_ShakeActiveList, 0, sizeof(void *));
    value = 1.0f;
    for (i = 0; i < 8; i++) {
        initShake(&owner->m_ShakeVars[i], value);
        CList__AddTail(&owner->m_ShakeFreeList, &owner->m_ShakeVars[i]);
    }
    CList__Construct(&owner->m_FreeList, 0, sizeof(void *));
    CList__Construct(&owner->m_ActiveList, 0, sizeof(void *));
    CList__Construct(&owner->m_FreeTextList, 0, sizeof(void *));
    owner->m_bOnlyUseMainCameraForSound = 1;
    Sound__SetListener(&owner->m_MainCamera);
    CCache__Flush(0);
    if (owner->m_pmeCameras) {
        CCache__Deallocate(0, owner->m_pmeCameras);
        owner->m_pmeCameras = NULL;
        owner->m_Cameras = NULL;
        owner->m_nCameras = 0;
    }
    CCache__Flush(0);
    if (owner->m_pmeTexts) {
        CCache__Deallocate(0, owner->m_pmeTexts);
        owner->m_pmeTexts = NULL;
        owner->m_Texts = NULL;
        owner->m_nTexts = 0;
    }
    if (count) {
        owner->m_nCameras = count;
        owner->m_pmeCameras = CCache__Allocate(0, count * sizeof(CCamera), 0x23, "Cameras");
        owner->m_Cameras = *(CCamera **) owner->m_pmeCameras;
        memset(owner->m_Cameras, 0, count * sizeof(CCamera));
        for (i = 0; i < owner->m_nCameras; i++) {
            owner->m_Cameras[i].m_Index = i;
            CList__AddTail(&owner->m_FreeList, &owner->m_Cameras[i]);
        }
        Sound__SetListener(owner->m_Cameras);
        owner->m_bOnlyUseMainCameraForSound = 0;
    }
    owner->m_nTexts = 16;
    owner->m_pmeTexts = CCache__Allocate(0, 0x400, 0x23, "Camera Texts");
    owner->m_Texts = *(CCameraText **) owner->m_pmeTexts;
    memset(owner->m_Texts, 0, 0x400);
    for (i = 0; i < owner->m_nTexts; i++) {
        CList__AddTail(&owner->m_FreeTextList, &owner->m_Texts[i]);
    }
}

s32 CCamera__Advance(sptr);
/* Manager block cleared and set up here (0x11F0 bytes): three lists and
 * an embedded sub-object at 0x40. */
typedef struct {
    /* 0x0000 */ s32 unk0;
    /* 0x0004 */ s32 unk4;
    /* 0x0008 */ s32 unk8;
    /* 0x000C */ u8 list0C[0x20 - 0xC];
    /* 0x0020 */ u8 list20[0x34 - 0x20];
    /* 0x0034 */ u8 pad34[0x38 - 0x34];
    /* 0x0038 */ s32 unk38;
    /* 0x003C */ u8 pad3C[0x40 - 0x3C];
    /* 0x0040 */ u8 sub40[0xF00 - 0x40];
    /* 0x0F00 */ s32 unkF00;
    /* 0x0F04 */ s32 unkF04;
    /* 0x0F08 */ s32 unkF08;
    /* 0x0F0C */ u8 listF0C[0x11E8 - 0xF0C];
    /* 0x11E8 */ s32 unk11E8;
    /* 0x11EC */ u8 pad11EC[0x11F0 - 0x11EC];
} Unk00468D6C; /* size 0x11F0 */

void CCameraPool__Construct_2(Unk00468D6C *arg0) {
    void *temp_s1;

    memset(arg0, 0, 0x11F0U);
    CList__Construct(arg0->list0C, 0, sizeof(void *));
    CList__Construct(arg0->list20, 0, sizeof(void *));
    CList__Construct(arg0->listF0C, 0, sizeof(void *));
    temp_s1 = arg0->sub40;
    arg0->unk11E8 = 1;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkF00 = 0;
    arg0->unkF04 = 0;
    arg0->unkF08 = 0;
    arg0->unk38 = 2;
    CCamera__Construct_2(temp_s1);
    CCamera__Advance((sptr) temp_s1);
}

struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
typedef struct Triple Triple;

void CCameraPool__ResetEmitterPools_2(Triple *group) {
    int i;
    for (i = 0; i < group->z; i++) {
        CEmitterPool__Construct_2((char *)(i * 0xEC0 + group->y + 0x558));
    }
    CEmitterPool__Construct_2((char *)group + 0x598);
}

/* Takes the first free camera, resets it and moves it to the active list. */
CCamera *CCameraPool__AllocCamera_2(CCameraPool *arg0) {
    CCamera *temp_s0;

    temp_s0 = (CCamera *) arg0->m_FreeList.m_pHead;
    if (temp_s0 != NULL) {
        CCamera__Construct_2(temp_s0);
        CList__Remove(&arg0->m_FreeList, temp_s0);
        CList__AddTail(&arg0->m_ActiveList, temp_s0);
    }
    return temp_s0;
}

void CCameraPool__FreeCamera_2(s32 arg0, s32 arg1) {
    CList__Remove(arg0 + 0x20, arg1);
    CList__AddHead(arg0 + 0xC, arg1);
}

void CEmitterPool__Construct_2(CEmitterPool *arg0) {
    sptr var_s0;
    s32 var_s1;

    CList__Construct(&arg0->m_ActiveList, 0, sizeof(void *));
    CList__Construct(&arg0->m_FreeList, 0, sizeof(void *));
    var_s1 = 0;
    var_s0 = (sptr) arg0;
    do {
        CList__AddTail(&arg0->m_FreeList, var_s0);
        var_s1 += 1;
        var_s0 += 0x228;
    } while (var_s1 < 4);
    arg0->m_ActiveMask = 0;
}
