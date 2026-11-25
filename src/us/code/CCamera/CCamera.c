#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0xC - 0x8];
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s8 unkE;
    /* 0x0F */ u8 padF[0x12 - 0xF];
    /* 0x12 */ s8 unk12;
} Unk0027BD10;

void CIntelligence__Init(Unk0027BD10 *arg0) {
    arg0->unk4 = 1;
    arg0->unk0 = 0;
    arg0->unkC = 0;
    arg0->unkE = 0;
    arg0->unk12 = -1;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80231F6C_de_13.c, func_802337D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/engine.h"

extern f32 Math_DecayToZero(f32 arg0, f32 arg1);

u32 CShakeAxis__Advance(StateRecAt10 *state) {
    f32 scale;
    f32 step;
    if (state->value > state->limit) {
        state->limit = state->value;
    }
    if (state->rate == 0.0f) {
        state->accumulator = 9.0e1f;
    }
    if (state->rate < state->limit + state->limit) {
        state->rate = state->limit + state->limit;
    }
    step = gFrameIncrement;
    scale = 3.0f;
    state->value = Math_DecayToZero(state->value, step * scale);
    state->limit = Math_DecayToZero(state->limit, gFrameIncrement * scale);
    switch (state->mode) {
    case 1:
        if (state->rate > 0.0f) {
            state->accumulator += (state->rate + state->rate) * (gFrameIncrement * scale);
            state->rate = Math_DecayToZero(state->rate, 6.0f);
        }
        break;
    case 0:
        if (state->rate > 0.0f) {
            state->rate = Math_DecayToZero(state->rate, 6.0f);
        }
        break;
    }
    return state->rate == 0.0f;
}

extern f32 Random_Blend(f32 arg0, f32 arg1);
extern float func_002982F0(float x);
void CShake__ApplyOffset(Effect33920 *arg0, CVector3 position, CVector3 *out);
extern f32 SmoothStep(f32 arg0);
extern void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);
extern f32 func_00298470(f32);

void CShake__ApplyOffset(Effect33920 *arg0, CVector3 position, CVector3 *out) {
    f32 distance;
    f32 value;
    AxisWave *wave;
    CVector3__Subtract(&position, &position, &arg0->origin);
    distance = func_00298470((position.x * position.x) +
                             (position.y * position.y) +
                             (position.z * position.z));
    if (distance < arg0->radius) {
        distance = SmoothStep(1.0f - (distance / arg0->radius));
        wave = &arg0->x;
        if (wave->mode == 0) goto x_sine;
        if (wave->mode != 1) goto x_zero;
        value = func_002982F0(wave->accumulator * 0.017453294f) * wave->rate;
        goto x_done;
x_sine:
        value = Random_Blend(-wave->rate, wave->rate);
        goto x_done;
x_zero:
        value = 0.0f;
x_done:
        out->x += value * distance;
        wave = &arg0->y;
        if (wave->mode == 0) goto y_sine;
        if (wave->mode != 1) goto y_zero;
        value = func_002982F0(wave->accumulator * 0.017453294f) * wave->rate;
        goto y_done;
y_sine:
        value = Random_Blend(-wave->rate, wave->rate);
        goto y_done;
y_zero:
        value = 0.0f;
y_done:
        out->y += value * distance;
        wave = &arg0->z;
        if (wave->mode == 0) goto z_sine;
        if (wave->mode != 1) goto z_zero;
        value = func_002982F0(wave->accumulator * 0.017453294f) * wave->rate;
        goto z_done;
z_sine:
        value = Random_Blend(-wave->rate, wave->rate);
        goto z_done;
z_zero:
        value = 0.0f;
z_done:
        out->z += value * distance;
    }
}

#include "structs/demo.h"

/* Reads count bits, most significant first. */
VM_TEXT static __inline__ s32 readBits(DemoStream *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->pos++;
        v |= (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

VM_TEXT void Demo_ReadRunLength(DemoStream *r) {
    s32 n = readBits(r, 3);

    if (n != 0) {
        r->frames = readBits(r, n) + 1;
    } else {
        r->frames = 1;
    }
}

VM_TEXT static __inline__ s32 readBit(DemoStream *r) {
    u32 pos = r->pos++;

    return (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
}

/* Reads a nine-bit code, its bits stored in a scrambled order, then decodes it. */
VM_TEXT void Demo_DecodeButtons(DemoStream *r) {
    r->value = readBit(r) << 15;
    r->value |= readBit(r) << 14;
    r->value |= readBit(r) << 4;
    r->value |= readBit(r) << 13;
    r->value |= readBit(r) << 3;
    r->value |= readBit(r) << 1;
    r->value |= readBit(r);
    r->value |= readBit(r) << 2;
    r->value |= readBit(r) << 8;
    Demo_ReadRunLength(r);
}

#include "structs/game.h"
#include "globals/game.h"

void Demo_DecodeStick(DemoStream *t);
void Demo_DecodeMatch(DemoStream *t);
extern void GameSettings_SetDefaults(GameSettings *setup);
extern s32 Random_Next(void);

VM_TEXT static __inline__ void initSetting(DemoStream *t, DemoHeader *config, s32 index, void (*handler)(), s32 value) {
    t->bits = (u8 *)config;
    t->player = index;
    t->frames = 1;
    t->decode = handler;
    t->unk18 = 0;
    t->unk1C = 0;
    t->pos = value;
}

/* Starts a match from config: sets up the per-player settings and the game setup. */
VM_TEXT void Demo_Start(DemoHeader *arg) {
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
            initSetting(&s->buttons[i], owner, i, Demo_DecodeButtons, config->buttonsStart[i]);
            initSetting(&s->stickX[i], owner, i, Demo_DecodeStick, config->stickXStart[i]);
            initSetting(&s->stickY[i], owner, i, Demo_DecodeStick, config->stickYStart[i]);
        }
    }
    initSetting(&s->match, owner, 0, Demo_DecodeMatch, config->matchStart);
    s->playerCount = config->playerCount;
    s->unk1B8 = 0;
    s->countdown = config->frames;
    s->time = 0;
    s->checksum = 0;
    gEngineApp_m_Settings.matchMode = config->matchMode;
    setup = &gEngineApp_m_Settings;
    GameSettings_SetDefaults(setup);
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

#include "structs/controller.h"

void CEngineApp__QuitToLegal(void *);
void CEngineApp__SetupFadeTo(void *, s32);

/* Demo_UpdateChecksum */
VM_TEXT static __inline__ void updateChecksum(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += gFrameCountGameplay * s->buttons[i].value * 0x12345678;
        s->checksum += gFrameCountGameplay * s->stickX[i].value * 0x87654321;
        s->checksum += gFrameCountGameplay * s->stickY[i].value * 0x12348765;
    }
    s->checksum += gFrameCountGameplay * s->match.value * 0x87651234;
}

VM_TEXT void Demo_Update(void) {
    DemoPlayer *s = &gDemoPlayer;

    s->fastForward = 0;
    s->time += gFrameIncrement;
    if (s->done == 0) {
        if (--s->countdown <= 0 || gMergedController.buttonsSingle != 0) {
            s->done = 1;
            CEngineApp__QuitToLegal(&gEngineApp);
            if (gMergedController.buttonsSingle != 0) {
                CEngineApp__SetupFadeTo(&gEngineApp, 8);
            }
        }
    }
    updateChecksum();
}

#include "structs/twin_demo.h"
extern Slot gControllers_buttons[]; /* local type: globals/controller.h declares it; this view (its type here) is what keeps the ROM's code */

VM_TEXT static __inline__ void fire(DemoStream *t) { t->decode(t); }

VM_TEXT static __inline__ void tick(DemoStream *t) { if (--t->frames <= 0) t->decode(t); }

VM_TEXT void Demo_UpdateStreams(void) {
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
    Demo_Update();
}

VM_TEXT void Demo_Reset(void) {
    gDemoPlayer.active = 0;
    gDemoPlayer.unk4 = 0;
    gDemoPlayer.playing = 0;
    gDemoPlayer.fastForward = 0;
    gDemoPlayer.index = 0;
    gDemoPlayer.saved = 0;
}

/* Ends a demo: puts back the game settings saved when it started. */
VM_TEXT void Demo_End(void) {
    DemoPlayer *s = &gDemoPlayer;

    if (s->saved != 0) {
        s->saved = 0;
        *(GameSettingsBlock *)&gEngineApp_m_Settings = s->settings;
    }
    s->active = 0;
    s->unk4 = 0;
    s->playing = 0;
}

#include "m2c_macros.h"

VM_TEXT void Demo_Enable(void) {
    gDemoPlayer.active = 1;
}

#include "globals/demo.h"

VM_TEXT void Demo_PlayNext(void) {
    DemoPlayer *state = &gDemoPlayer;
    s32 temp_v0;

    if (state->active != 0) {
        Demo_Start(gDemoTable[state->index]);
        temp_v0 = state->index + 1;
        state->index = temp_v0;
        if (gDemoTable[temp_v0] == NULL) {
            state->index = 0;
        }
    }
}

VM_TEXT void DemoStream_Init(DemoStream *t, sptr bits, s32 pos, s32 player, void (*decode)(DemoStream *), s32 unk18, s32 unk1C) {
    t->bits = (u8 *)bits;
    t->pos = pos;
    t->player = player;
    t->frames = 1;
    t->decode = decode;
    t->unk18 = unk18;
    t->unk1C = unk1C;
}

/* Something with a handler at 0x14 that this calls. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 (*handler)();
} HandlerHolder;

VM_TEXT void DemoStream_Decode(HandlerHolder *arg0) {
    arg0->handler();
}

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 (*callback)();
} Unk0042F650;

VM_TEXT void DemoStream_CallHandler2(Unk0042F650 *arg0) {
    arg0->callback();
}


VM_TEXT void DemoStream_RunHandler2(Job *arg0) {
    arg0->result = arg0->func();
    arg0->unkC = 0;
}

/* Bit `bit` of a big-endian bit array. */
VM_TEXT s32 Bits_Test(u8 *bits, u32 bit) {
    u8 *p = bits + (bit >> 3);
    s32 mask = 0x80 >> (bit & 7);

    return (*p & mask) != 0;
}

/* Reads the next bit. */
VM_TEXT s32 DemoStream_ReadBit(DemoStream *r) {
    u32 bit = r->pos++;

    return (r->bits[bit >> 3] & (0x80 >> (bit & 7))) != 0;
}

/* A timer whose durations are read from a bit stream (see Demo_ReadRunLength). */

/* Reads count bits, most significant first. */
VM_TEXT s32 DemoStream_ReadBits(DemoStream *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->pos++;
        v |= (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

VM_TEXT void Demo_DecodeStick(DemoStream *t) {
    t->value = readBits(t, 8);
    Demo_ReadRunLength(t);
}

VM_TEXT void Demo_DecodeMatch(DemoStream *t) {
    t->value = readBits(t, 2) + 2;
    Demo_ReadRunLength(t);
}

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 countdown;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 (*callback)();
} Unk0042F838;

VM_TEXT void DemoStream_Tick(Unk0042F838 *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->countdown - 1;
    arg0->countdown = temp_v0;
    if (temp_v0 <= 0) {
        arg0->callback();
    }
}

/* Folds the timers' values into the state's checksum. */
VM_TEXT void Demo_UpdateChecksum(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += gFrameCountGameplay * s->buttons[i].value * 0x12345678;
        s->checksum += gFrameCountGameplay * s->stickX[i].value * 0x87654321;
        s->checksum += gFrameCountGameplay * s->stickY[i].value * 0x12348765;
    }
    s->checksum += gFrameCountGameplay * s->match.value * 0x87651234;
}

#include "globals/frame.h"

VM_TEXT void Demo_UpdateMatchStream(void) {
    DemoPlayer *s = &gDemoPlayer;
    DemoStream *t = &s->match;

    if (s->fastForward != 0 || --t->frames <= 0) {
        t->decode(t);
    }
    gNextTicks = s->match.value;
}

#include "structs/camera.h"
#include "globals/render.h"
#include "protos/list.h"
#include "structs/twin_demo.h"
#include "globals/camera.h"
#ifdef TARGET_PORT
int T2Port_LegacyShake(void); /* port/game/t2debug.c: T2_LEGACY_SHAKE */
#endif

/* gCameraVisBits and gScreenClearTime after it, seen as one structure: the store below is addressed off
 * gCameraVisBits (one %hi for both), as a word. */
typedef struct {
    /* 0x0 */ u32 m_VisBits;
    /* 0x4 */ s32 m_ScreenClearTime;
} CCameraVisPair;
void CCameraViewParams__Construct(CCameraViewParams *);
void CMtxF__Identity(void *);
void CCamera__ResetWarp(CCamera *cam);
void CCamera__SetMode(CCamera *arg0, s16 arg1);
void MenuPool_Construct(void *);
void CCamera__ConstructSkySystem(CCamera *cam);
#include "structs/emitter.h"
void CEmitterPool__Construct(CEmitterPool *arg0);

VM_TEXT void CCamera__Construct(CCamera *view) {
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
#ifdef TARGET_PORT
    /* Round 16 e1: f2 is CShake at the N64 offsets (m_vPos at 0x08, m_Radius at 0x14). On the host
     * the two list pointers are 8 bytes each: these four stores hit m_pNext and m_vPos.x/y, and
     * m_Radius stayed 0, so CShake__ApplyOffset never drew its three Random_Blend for each camera
     * each frame (the N64 does: its m_Radius is 1). The same stores at the members. */
    if (T2Port_LegacyShake()) {
        /* DEBUG-ONLY (T2_LEGACY_SHAKE=1): the stores as rounds 1-15 made them, so that the scripted
         * whole-game route (tools/port/fullrun.py, tuned on that RNG stream) replays unchanged */
        f2->unk8 = zero;
        f2->unkC = zero;
        f2->unk10 = zero;
        f2->unk14 = one;
    } else {
        view->m_Shake.m_vPos.x = zero;
        view->m_Shake.m_vPos.y = zero;
        view->m_Shake.m_vPos.z = zero;
        view->m_Shake.m_Radius = one;
    }
#else
    f2->unk8 = zero;
    f2->unkC = zero;
    f2->unk10 = zero;
    f2->unk14 = one;
#endif
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
    MenuPool_Construct(view->m_MenuPool);
    CEmitterPool__Construct((struct CEmitterPool *) view->m_SkySystem);
    CMtxF__Identity(view->m_mfFaceScreen);
    CMtxF__Identity(view->m_mfFaceScreenYAxisUp);
    view->m_FlashMode = 0;
    ((CCameraVisPair *) &gCameraVisBits)->m_ScreenClearTime = 0;
    CList__Construct(&view->m_ActiveTextList, T2_OFFSETOF(CCameraText, m_pLast), T2_OFFSETOF(CCameraText, m_pNext));
    CCamera__ConstructSkySystem(view);
}

/* Runs the screen flash (CCamera__StartFlash starts it): rises over m_AttackTime
 * frames to m_MaxOpacity, holds for m_SustainTime, fades over m_DecayTime,
 * the opacity in m_FlashColor[3]. */
void CCamera__UpdateFlash(CCamera *cam) {
    switch ((u32) cam->m_FlashMode) {     /* unsigned: "not 1, below 2" is then just 0 */
        case 0:                 /* none */
            break;
        case 1:                 /* rising */
            if (cam->m_AttackTime <= cam->m_FlashTimer) {
                cam->m_FlashMode = 2;
                cam->m_FlashTimer = 0.0f;
                goto hold;
            }
            cam->m_FlashColor[3] = (u32) (cam->m_FlashTimer / (cam->m_AttackTime + 1) * cam->m_MaxOpacity);
            break;
        case 2:                 /* holding */
        hold:
            if (!(cam->m_SustainTime <= cam->m_FlashTimer)) {
                cam->m_FlashColor[3] = cam->m_MaxOpacity;
                break;
            }
            cam->m_FlashMode = 3;
            cam->m_FlashTimer = 0.0f;
        case 3:                 /* fading */
            if (cam->m_DecayTime <= cam->m_FlashTimer) {
                cam->m_FlashMode = 0;
                cam->m_FlashTimer = 0.0f;
                break;
            }
            cam->m_FlashColor[3] = (u32) (cam->m_MaxOpacity - cam->m_FlashTimer / (cam->m_DecayTime + 1) * cam->m_MaxOpacity);
            break;
    }
    cam->m_FlashTimer += gFrameIncrement;
}

#include "structs/player.h"
#include "globals/player.h"

void CEngineApp__DrawFilledRect(void *, u8 *, s32, s32, s32, s32);
s32 Cinema_PathPlaying(void);
void CPlayerPool__DrawHuds(CPlayerPool *list, CCamera *cam);
#ifdef TARGET_PORT
void Cinema_DrawOverlay(CCamera *cam); /* returns void: on MIPS its $v0 is the cinema flag or Cinema_DrawFade's; every caller of CCamera__DrawTints drops it */
#else
s32 Cinema_DrawOverlay(CCamera *cam);
#endif

/* The tints over a camera's view (LibTEngine's CCamera DrawTints): the fog
 * colour while its player is under water (AI flags 0x880) or a forced
 * tint is up, then the screen flash, then the red flash of a hit fading at
 * m_FlashDec a frame. */
s32 CCamera__DrawTints(CCamera *cam) {
    CPlayer *player;
    u32 flags;
    u8 tint[4];
    u8 hurt[4];
    u8 *rgba;
    s32 i;

    /* CCamera__GetPlayer, inlined */
    for (player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == (void *) cam) {
            goto found;
        }
    }
    player = NULL;
found:
    if (player != NULL) {
        flags = player->m_CharacterActor.m_AI.m_dwFlags;
        sTintFlagsPrev2 = sTintFlagsPrev;
        sTintFlagsPrev = sTintFlags;
        sTintFlags = flags;
        if ((flags & 0x880) || gDeathTint != 0) {
            tint[0] = cam->m_FogColor[0] & 0xE0;
            tint[1] = cam->m_FogColor[1] & 0xE0;
            tint[2] = cam->m_FogColor[2] & 0xE0;
            tint[3] = cam->m_FogColor[3] & 0xE0;
            if (gDeathTint != 0) {
                for (i = 0; i < 4; i++) {
                    tint[i] = gDeathTintColor[i];
                }
            }
            if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
                tint[0] >>= 3;
                tint[1] >>= 3;
                tint[2] >>= 3;
            }
            CEngineApp__DrawFilledRect(&gEngineApp, tint, (s32) cam->m_Viewport.m_ScreenPosX, (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        CCamera__UpdateFlash(cam);
        if (cam->m_FlashMode != 0) {
            CEngineApp__DrawFilledRect(&gEngineApp, cam->m_FlashColor, (s32) cam->m_Viewport.m_ScreenPosX,
                          (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        rgba = hurt;
        if (cam->m_Flash > cam->m_FlashDec) {
            cam->m_Flash -= (u32) (cam->m_FlashDec * gFrameIncrement);
        } else {
            cam->m_Flash = 0;
        }
        if (cam->m_Flash != 0) {
            rgba[0] = 0xFF;
            hurt[1] = 0;
            hurt[2] = 0;
            hurt[3] = cam->m_Flash;
            CEngineApp__DrawFilledRect(&gEngineApp, hurt, (s32) cam->m_Viewport.m_ScreenPosX, (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        if (Cinema_PathPlaying() == 0 && gEngineApp_m_Game.m_Settings.mode == 13) {
            CPlayerPool__DrawHuds(&gEngineApp_m_Game.m_PlayerList, cam);
        }
#ifdef TARGET_PORT
        Cinema_DrawOverlay(cam);
        return 0;
#else
        return Cinema_DrawOverlay(cam);
#endif
    }
}

/* float -> u32 as the R4300 does it (cvt.w.s: a negative value wraps). arm64's fcvtzu saturates
 * negatives to 0 (MACOS.md section 9, tools/port/f2ucheck.py); x86-64 happens to wrap. */
#ifdef TARGET_PORT
#define T2_F2U(x) ((u32)(s32)(x))
#else
#define T2_F2U(x) ((u32)(x))
#endif
#include "structs/light.h"

f32 func_002974C0(f32 x);       /* cosine */
f32 func_002982F0(f32 x);       /* sine */
void CVector3__Normalize(f32 *v);     /* normalise */

/* Cycles a light and the camera's fog colour with the frame counter: the
 * light's colours pulse on sine waves, its direction turns, and the fog
 * drifts between dark and light tones (rounded to 5 bits a channel). */
void CCamera__CycleLightAndFog(CCamera *cam) {
    CVector3 direction;
    f32 phaseScale;
    f32 ambientScale;
    f32 ambient;
    f32 diffuse;
    f32 red;
    f32 green;
    f32 blue;
    s32 channel;
    s8 redByte;
    s8 greenByte;
    s8 blueByte;

    phaseScale = 1.0f;
    channel = 0;
    ambientScale = phaseScale;
    do {
        ambient = (ambientScale * (func_002982F0(
            phaseScale * (f32) gFrameCountGameplay * ((f32) (channel + 2) * 4.0f) *
            0.01745329424738884f) * 16.0f)) + 16.0f + 48.0f;
        gSceneLights.m_AmbColCopy[channel] = gSceneLights.m_AmbCol[channel] = (u32) ambient;
        diffuse = (((func_002982F0(
            phaseScale * (f32) gFrameCountGameplay * ((f32) (channel + 2) * 4.0f) *
            0.01745329424738884f) * 64.0f) + 255.0f) - 64.0f) - 1.0f;
        gSceneLights.m_ColCopy[channel] = gSceneLights.m_Col[channel] = (u32) diffuse;
        channel += 1;
    } while (channel < 3);
    direction.x = func_002982F0((f32) (gFrameCountGameplay * 6) * 0.01745329424738884f);
    direction.y = func_002982F0((f32) (gFrameCountGameplay * 8) * 0.01745329424738884f);
    direction.z = func_002974C0((f32) (gFrameCountGameplay * 10) * 0.01745329424738884f);
    CVector3__Normalize(&direction.x);
    direction.x *= 120.0f;
    direction.y *= 120.0f;
    direction.z *= 120.0f;
    gSceneLights.m_Dir[0] = T2_F2U(direction.x);
    gSceneLights.m_Dir[1] = T2_F2U(direction.y);
    gSceneLights.m_Dir[2] = T2_F2U(direction.z);
    red = (func_002982F0(2.0f * (phaseScale * (f32) gFrameCountGameplay) * 0.06981317698955536f) * 64.0f) + 128.0f;
    redByte = (u32) red & 0xF8;
    cam->m_FogColor[0] = redByte;
    cam->m_LastFogColor[0] = redByte;
    green = (func_002982F0(phaseScale * (f32) gFrameCountGameplay * 0.20943953096866608f) * 64.0f) + 128.0f;
    greenByte = (u32) green & 0xF8;
    cam->m_FogColor[1] = greenByte;
    cam->m_LastFogColor[1] = greenByte;
    blue = (func_002982F0(phaseScale * (f32) gFrameCountGameplay * 0.27925270795822144f) * 64.0f) + 128.0f;
    blueByte = (u32) blue & 0xF8;
    cam->m_FogColor[2] = blueByte;
    cam->m_LastFogColor[2] = blueByte;
}

#include "globals/menu.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* What a region set gives a camera (the record CScene__GetRegionInfo finds). */
typedef struct {
    /* 0x00 */ u8 m_FogColor[4];
    /* 0x04 */ u8 m_HeldFogColor[4];        /* under water, or as the player's flags ask */
    /* 0x08 */ u32 m_FogStart;
    /* 0x0C */ u32 m_HeldFogStart;
    /* 0x10 */ f32 m_FarClip;
    /* 0x14 */ f32 m_FieldOfView;
    /* 0x18 */ f32 m_HeldFarClip;
    /* 0x1C */ f32 m_HeldFieldOfView;
    /* 0x20 */ f32 m_Darkness;
    /* 0x24 */ f32 m_BlendTime;             /* in 15ths of a second */
    /* 0x28 */ u8 _pad28[0x32];
    /* 0x5A */ u8 m_Music2;
    /* 0x5B */ u8 m_Music1;
} CRegionSetAttributes;
s32 Cinema_OverridesMusic1(void);
s32 Cinema_OverridesMusic2(void);
void Sound__SetAmbientTrack(u8 track);
void Music_SetTrack(u8 track);
f32 GameObject_GetViewWaterHeight(CPlayer *player);

/* CCamera__GetPlayer, inlined: the player whose camera this is, or NULL. */
static __inline__ CPlayer *GetPlayer(CCamera *cam) {
    CPlayer *player;
    CPlayer *node;

    for (player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == cam) {
            return player;
        }
    }
    return NULL;
}

/* The region set a camera is in decides its fog colour, fog start, far clip
 * and field of view (LibTEngine's CCamera AdvanceAttributes); when the set
 * changes the camera blends from the old values to the new ones over the
 * set's blend time. In game mode 13 the first camera also starts the set's
 * music. */
void CCamera__AdvanceAttributes(CCamera *cam) {
    u8 target[4];
    CRegionSetAttributes *rec;
    CPlayer *player;
    CPlayer *node;
    u16 *key;
    u32 hold, freeze;
    u32 fogStart;
    f32 farClip, fov;
    f32 t, k, kn, last, v;
    f32 darkness;
    s32 dark;
    u32 c8;
    u32 r;
    s32 i;

    key = (u16 *) cam->m_ViewParams.m_pCurrentRegion;
    if (key == NULL) {
        return;
    }
    rec = CScene__GetRegionInfo(&gScene, key);
    if (rec == NULL) {
        return;
    }
    if (gEngineApp_m_Settings.paused == 0 && cam == gEngineApp_m_Game_m_CameraPool.m_Cameras && gEngineApp_m_Settings_mode == 13 && gEngineApp_m_Settings_nextMode == gEngineApp_m_Settings_mode) {
        if (Cinema_OverridesMusic1() == 0) {
            Sound__SetAmbientTrack(rec->m_Music1);
        }
        if (Cinema_OverridesMusic2() == 0) {
            Music_SetTrack(rec->m_Music2);
        }
    }
    freeze = hold = 0;
    node = GetPlayer(cam);
    if (node != NULL) {
        hold = node->m_ModeFlags & 0x40;
        hold |= node->m_CharacterActor.m_AI.m_dwFlags & 0x80;
        freeze = node->m_CharacterActor.m_AI.m_dwFlags & 0x800;
        player = node;
    } else {
        player = NULL;
    }
    if (*key != cam->m_nCurrentRegionSet || hold != 0 || freeze != 0) {
        if (cam->m_nCurrentRegionSet == -1 || hold != 0 || freeze != 0) {
            cam->m_cRegionSetBlend = 1.0f;
        } else {
            cam->m_cRegionSetBlend = 0.0f;
        }
        cam->m_nCurrentRegionSet = *key;
        for (i = 0; i < 4; i++) {
            cam->m_LastFogColor[i] = cam->m_FogColor[i];
        }
        cam->m_LastFarClip = cam->m_FarClip;
        cam->m_LastFogStart = cam->m_FogStart;
        cam->m_LastFieldOfView = cam->m_FieldOfView;
    }
    if (hold != 0 || freeze != 0) {
        if (freeze != 0) {
            for (i = 0; i < 3; i++) {
                target[i] = rec->m_HeldFogColor[i];
            }
            target[3] = 0;
        } else {
            darkness = (player != NULL) ? GameObject_GetViewWaterHeight(player) : rec->m_Darkness;
            dark = (s32) darkness / 16;
            for (i = 0; i < 3; i++) {
                target[i] = (rec->m_HeldFogColor[i] - dark < 0) ? 0 : rec->m_HeldFogColor[i] - dark;
            }
            target[3] = (rec->m_HeldFogColor[3] + dark > 0xC0) ? 0xC0 : rec->m_HeldFogColor[3] + dark;
        }
        fogStart = rec->m_HeldFogStart;
        do {
            farClip = rec->m_HeldFarClip;
            fov = rec->m_HeldFieldOfView;
        } while (0);
    } else {
        if (gCameraPoolHideViews != 0) {
            u8 *p;

            for (i = 3, p = &target[3]; i >= 0; i--) {
                *p-- = 0;
            }
        } else {
            for (i = 0; i < 3; i++) {
                target[i] = rec->m_FogColor[i];
            }
            target[3] = 0;
        }
        fogStart = rec->m_FogStart;
        farClip = rec->m_FarClip;
        fov = rec->m_FieldOfView;
    }
    if (gFrameCountGameplay == 0 || rec->m_BlendTime == 0.0f) {
        cam->m_cRegionSetBlend = 1.0f;
    } else {
        t = cam->m_cRegionSetBlend + gFrameIncrement / (rec->m_BlendTime * 15.0f);
        if (1.0f < t) {
            t = 1.0f;
        }
        cam->m_cRegionSetBlend = t;
    }
    t = cam->m_cRegionSetBlend;
    kn = t * (t * -2.0f);
    kn = kn * t;
    t = t * (t * 3.0f);
    k = kn + t;
    for (i = 0; i < 4; i++) {
#ifdef TARGET_PORT /* 0x50C/0x508: the N64 offsets of m_LastFogColor/m_FogColor */
        last = cam->m_LastFogColor[i];
        v = last + k * ((f32) target[i] - last);
        c8 = (u32) (((255.0f < v) ? 255.0f : v) < 0.0f ? 0.0f : ((255.0f < v) ? 255.0f : v));
        cam->m_FogColor[i] = c8;
#else
        u8 *colour = (u8 *) cam + i;

        last = colour[0x50C];
        v = last + k * ((f32) target[i] - last);
        c8 = (u32) (((255.0f < v) ? 255.0f : v) < 0.0f ? 0.0f : ((255.0f < v) ? 255.0f : v));
        colour[0x508] = c8;
#endif
    }
    /* The fog start blends as a float, clamped to [0, 995]. */
#define FOG_START ((f32) (u32) cam->m_LastFogStart + k * ((f32) fogStart - (f32) (u32) cam->m_LastFogStart))
    r = (u32) (((995.0f < FOG_START) ? 995.0f : FOG_START) < 0.0f ? 0.0f : ((995.0f < FOG_START) ? 995.0f : FOG_START));
#undef FOG_START
    cam->m_FogStart = r;
    if (cam->m_ViewParams.m_ZoomBlend > 0.0f) {
        farClip = farClip + cam->m_ViewParams.m_ZoomBlend * 1024.0f;
    }
    cam->m_FarClip = cam->m_LastFarClip + k * (farClip - cam->m_LastFarClip);
    cam->m_FieldOfView = cam->m_LastFieldOfView + k * (fov - cam->m_LastFieldOfView);
}

#include "gfx.h"
#include "gbi_t2.h"
#include "globals/math.h"
#ifdef TARGET_PORT /* the camera's region is a host CGameRegion (pointer-wide vertex and neighbour
                    * slots): at 0x1C the view read the high half of m_pNeighbours[1], 0, and the
                    * gameplay camera's visibility mask hid every static object */
#include "structs/region.h"
typedef CGameRegion CFloor;
#else
typedef struct {
    /* 0x00 */ u8 _pad0[0x1C];
    /* 0x1C */ u32 m_VisBits;
} CFloor;
#endif
/* The camera pool, seen from its main camera. */
#ifdef TARGET_PORT /* m_MainCamera's host offset (0x40 on the N64): with 0x40, the main camera test in
                    * CCamera__DisplayListSetup failed and it cleared the frame over the scene */
#define gEngineApp_m_Game_m_CameraPool (*(CCameraPool *) ((u8 *) &gEngineApp_m_Game_m_CameraPool.m_MainCamera - __builtin_offsetof(CCameraPool, m_MainCamera)))
#else
#define gEngineApp_m_Game_m_CameraPool (*(CCameraPool *) ((u8 *) &gEngineApp_m_Game_m_CameraPool.m_MainCamera - 0x40))
#endif

void guLookAtF(CMtxF m, f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ);
void Cinema_UpdateCameraIfActive(CCamera *cam);
void CCamera__UpdateTremor(CCamera *cam, CVector3 *offset);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, CMtxF m);
void CMtxF__SetTranslate(CMtxF m, f32 x, f32 y, f32 z);
void CMtxF__MultiplyAffine(CMtxF out, CMtxF a, CMtxF b);
void CMtxF__TransformPoint(CMtxF m, CVector3 *in, CVector3 *out);
void CMtxF__GetTranslation(CMtxF m, CVector3 *pos);
void CMtxF__InvertAffine(CMtxF out, CMtxF m);
void CMtxF__SetRotateY(CMtxF m, f32 angle);
void CMtxF__Copy(CMtxF out, CMtxF m);
void Math_EaseToward(f32 *value, f32 target, f32 rate);
f32 Cinema_GetCameraFov(CCamera *cam);
f32 func_00298470(f32 x);       /* sqrtf */
f32 Math_AcosLookup(f32 x);
f32 Cinema_GetFarClip(void);
void guPerspectiveF(CMtxF m, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void CMtxF__SetScale(CMtxF m, f32 x, f32 y, f32 z);
void CMtxF__Multiply(CMtxF out, CMtxF a, CMtxF b);
void CMtxF__ClampEntries(CMtxF m);
void CMtxF__ToMtx(CMtxF m, Mtx *out);
void CMtxF__RotateX(CMtxF m, f32 angle);
void CMtxF__RotateY(CMtxF m, f32 angle);
void CMtxF__TransformPoints(CMtxF m, CVector3 *in, CVector3 *out, u32 count);
void Frustum_FromCorners(CPlane *planes, CVector3 *eye, CVector3 *c0, CVector3 *c1, CVector3 *c2, CVector3 *c3);
void BoundingBox_FromPoints(CVector3 *box, s32 count, CVector3 *points);
void CEmitterPool__Advance(void *sky, CCamera *cam);
void MenuPool_Update(void *menus);
f32 CGameRegion__GetHeight(struct CGameRegion *, f32, f32);
#define MIN1(x) (((x) > 1.0f) ? 1.0f : (x))

/* Advances a camera (LibTEngine's CCamera Advance): see docs/CAMERA.md. */
void CCamera__Advance(CCamera *cam) {
    CMtxF mLookAt, mRot, mYOffset, mGround, mTrans, mPi, mInv, mView, m, mOrient;
    CMtxF mProj, mFlip, mLight, mOnScreen;
    CQuaternion qX, qY, qZ, qXY, q;
    CVector3 corners[5];
    CVector3 tremor, shake, shakeOffset;
    CQuaternion qGround;
    CPlayer *node;
    CPlayer *player;
    LookAt *lookAt;
    void (*update)(CCamera *);
    CShake *s;
    CFloor *floor;
    f32 rotX, rotZ, rotY, rotYOffset, posX, posY, posZ, yOffset, zoomBlend, zoomFov;
    f32 half, a, sn, swim, inv;
    f32 fov, aspect, t, ty, far, halfX, halfY, radius, r15, v;
    f32 x, y, z, height;
    /* Block-local temporaries: separate variables keep each in its own block (GCC's local allocator). */
    f32 cornerX, cornerY, yawCam, yawPath, ang, above, depth;
    f32 boxX, boxY, boxZ;
    s32 n;

    for (node = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; node != NULL; node = node->m_pNext) {
        if (node->m_pCamera == cam) {
            player = node;
            goto found;
        }
    }
    player = NULL;
found:
    cam->m_Time += gFrameIncrement;
    CCamera__AdvanceAttributes(cam);
    if (Cinema_PathPlaying() == 0) {
        update = gCameraModeTable[cam->m_Mode].update;
        if (update != NULL) {
            update(cam);
        }
    }
    lookAt = &cam->m_LookAt[gEvenOdd];
    guLookAtF(mLookAt, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    Cinema_UpdateCameraIfActive(cam);
    rotX = cam->m_ViewParams.m_vRotOffset.x;
    rotZ = cam->m_ViewParams.m_vRotOffset.z;
    posX = cam->m_ViewParams.m_vPos.x;
    posY = cam->m_ViewParams.m_vPos.y;
    posZ = cam->m_ViewParams.m_vPos.z;
    rotY = cam->m_ViewParams.m_RotY;
    rotYOffset = cam->m_ViewParams.m_vRotOffset.y;
    yOffset = cam->m_ViewParams.m_YPosOffset;
    qGround = cam->m_ViewParams.m_qGround;
    zoomBlend = cam->m_ViewParams.m_ZoomBlend;
    zoomFov = cam->m_ViewParams.m_ZoomFov;
    if (Cinema_PathPlaying() == 0) {
        CCamera__UpdateTremor(cam, &tremor);
        rotZ += tremor.z;
        rotX += tremor.x;
        posY += tremor.y;
        half = 0.5f;
        a = rotZ * half;
        sn = func_002982F0(a);
        gQuaternionSin = sn;
        qZ.x = 0.0f;
        qZ.y = 0.0f;
        qZ.z = sn;
        qZ.w = func_002974C0(a);
        a = rotX * half;
        sn = func_002982F0(a);
        gQuaternionSin = sn;
        qX.x = sn;
        qX.y = 0.0f;
        qX.z = 0.0f;
        qX.w = func_002974C0(a);
        yawCam = (rotY + rotYOffset) * half;
        sn = func_002982F0(yawCam);
        gQuaternionSin = sn;
        qY.x = 0.0f;
        qY.y = sn;
        qY.z = 0.0f;
        qY.w = func_002974C0(yawCam);
        CQuaternion__Multiply(&qXY, &qX, &qY);
        CQuaternion__Multiply(&cam->m_qViewAlignNoZ, &qXY, &qGround);
        CQuaternion__Multiply(&cam->m_qViewAlign, &qZ, &cam->m_qViewAlignNoZ);
        CQuaternion__Multiply(&q, &qZ, &qXY);
        CQuaternion__ToMatrix(&q, mRot);
        CMtxF__SetTranslate(mYOffset, 0.0f, yOffset, 0.0f);
        CQuaternion__ToMatrix(&qGround, mGround);
        shake.x = 0.0f;
        shake.y = 0.0f;
        shake.z = 0.0f;
        CShake__ApplyOffset(&cam->m_Shake, cam->m_Shake.m_vPos, &shake);
        CShakeAxis__Advance(&cam->m_Shake.m_XAxis);
        CShakeAxis__Advance(&cam->m_Shake.m_YAxis);
        CShakeAxis__Advance(&cam->m_Shake.m_ZAxis);
        if (cam != &gEngineApp_m_Game_m_CameraPool.m_MainCamera) {
            for (s = (CShake *) gEngineApp_m_Game_m_CameraPool.m_ShakeActiveList.m_pHead; s != NULL; s = s->m_pNext) {
                CShake__ApplyOffset(s, cam->m_ViewParams.m_vPos, &shake);
            }
        }
        CMtxF__MultiplyAffine(m, mRot, mGround);
        CMtxF__TransformPoint(m, &shake, &shakeOffset);
        posX += shakeOffset.x;
        posY += shakeOffset.y;
        posZ += shakeOffset.z;
        CMtxF__MultiplyAffine(m, mRot, mYOffset);
        CMtxF__MultiplyAffine(mOrient, m, mGround);
        CMtxF__SetTranslate(mTrans, posX, posY, posZ);
        CMtxF__MultiplyAffine(cam->m_mfViewOrient, mOrient, mTrans);
    } else {
        half = 0.5f;
        a = rotZ * half;
        sn = func_002982F0(a);
        gQuaternionSin = sn;
        qZ.x = 0.0f;
        qZ.y = 0.0f;
        qZ.z = sn;
        qZ.w = func_002974C0(a);
        a = rotX * half;
        sn = func_002982F0(a);
        gQuaternionSin = sn;
        qX.x = sn;
        qX.y = 0.0f;
        qX.z = 0.0f;
        qX.w = func_002974C0(a);
        yawPath = (rotY + rotYOffset) * half;
        sn = func_002982F0(yawPath);
        gQuaternionSin = sn;
        qY.x = 0.0f;
        qY.y = sn;
        qY.z = 0.0f;
        qY.w = func_002974C0(yawPath);
        CQuaternion__Multiply(&qXY, &qX, &qY);
        cam->m_qViewAlignNoZ = qXY;
        CQuaternion__Multiply(&cam->m_qViewAlign, &qZ, &cam->m_qViewAlignNoZ);
        CQuaternion__ToMatrix(&qXY, mRot);
        CMtxF__SetTranslate(mTrans, posX, posY, posZ);
        CMtxF__MultiplyAffine(cam->m_mfViewOrient, mRot, mTrans);
    }
    CMtxF__GetTranslation(cam->m_mfViewOrient, &cam->m_vPos);
    cam->m_vRotation.x = rotX;
    cam->m_vRotation.y = rotY;
    cam->m_vRotation.z = rotZ;
    CMtxF__InvertAffine(cam->m_mfInverseViewOrient, cam->m_mfViewOrient);
    CMtxF__SetRotateY(mPi, 3.1415927f);
    CMtxF__MultiplyAffine(m, mPi, cam->m_mfViewOrient);
    CMtxF__InvertAffine(mInv, m);
    CMtxF__MultiplyAffine(mView, mInv, mLookAt);
    CMtxF__Copy(cam->m_mfView, mView);
    if (player != NULL && (player->m_ModeFlags & 0x40) && cam->m_ViewParams.m_nType == 0 && Cinema_PathPlaying() == 0) {
        cam->m_XScale = MIN1(cam->m_WaterWobbleXScale * 0.5f * (func_002982F0(cam->m_WaterWobbleXAngle) + 1.0f) + 0.8f);
        cam->m_YScale = MIN1(cam->m_WaterWobbleYScale * 0.5f * (func_002982F0(cam->m_WaterWobbleYAngle) + 1.0f) + 0.9f);
        cam->m_WaterWobbleXAngle += gFrameIncrement * cam->m_WaterWobbleXSpeed;
        cam->m_WaterWobbleYAngle += gFrameIncrement * cam->m_WaterWobbleYSpeed;
        swim = player->m_TotalSpeed * 2.5f;
        if (swim < 0.0f) {
            swim = 0.0f;
        } else if (1.0f < swim) {
            swim = 1.0f;
        }
        Math_EaseToward(&cam->m_WaterWobbleXScale, 0.1f, 0.5f);
        Math_EaseToward(&cam->m_WaterWobbleYScale, 0.05f, 0.5f);
        inv = 1.0f - swim;
        Math_EaseToward(&cam->m_WaterWobbleXSpeed, swim * 0.104719765f + inv * 0.06981318f, 0.16666667f);
        Math_EaseToward(&cam->m_WaterWobbleYSpeed, swim * 0.087266475f + inv * 0.03490659f, 0.16666667f);
    } else {
        cam->m_YScale = 1.0f;
        cam->m_XScale = 1.0f;
    }
    if (Cinema_PathPlaying() != 0) {
        fov = Cinema_GetCameraFov(cam);
    } else {
        fov = cam->m_FieldOfView + zoomBlend * (zoomFov - cam->m_FieldOfView);
    }
    fov = fov * 0.017453294f;
    aspect = cam->m_Viewport.m_Width / cam->m_Viewport.m_Height;
    ty = func_002982F0(fov) / func_002974C0(fov);
    t = ty * 1.2792794f;
    ty = t / aspect;
    ty = ty / cam->m_YScale;
    t = t / cam->m_XScale;
    fov = Math_AcosLookup(func_00298470(1.0f / (ty * ty + 1.0f)));
    fov = fov * 57.295776f;
    far = cam->m_FarClip;
    aspect = t / ty;
    cam->m_AspectRatio = aspect;
    cam->m_Fov = fov;
    halfX = far * t;
    halfY = far * ty;
    if (Cinema_PathPlaying() != 0) {
        n = Cinema_GetFarClip();
        if (n > 0) {
            cam->m_FarClip = n;
        }
    }
    guPerspectiveF(mProj, &cam->m_PerspNorm, fov, aspect, 16.0f, cam->m_FarClip, 1.0f - zoomBlend * 0.95f);
    CMtxF__SetScale(mFlip, -1.0f, 1.0f, 1.0f);
    CMtxF__Multiply(m, mView, mProj);
    CMtxF__Multiply(cam->m_mfProjection, m, mFlip);
    CMtxF__ClampEntries(cam->m_mfProjection);
    CMtxF__ToMtx(cam->m_mfProjection, &cam->m_mProjection[gEvenOdd]);
    CMtxF__Copy(mLight, mView);
    if (gEngineApp_m_Game.m_Settings.mode == 4) {
        CMtxF__RotateX(mLight, cam->m_Time * 0.010908309f);
        CMtxF__RotateY(mLight, cam->m_Time * 0.008726647f);
    }
    CMtxF__ClampEntries(mLight);
    v = mLight[0][0] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x08] = (s32) v;
    v = mLight[1][0] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x09] = (s32) v;
    v = mLight[2][0] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x0A] = (s32) v;
    v = mLight[0][1] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x18] = (s32) v;
    v = mLight[1][1] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x19] = (s32) v;
    v = mLight[2][1] * 128.0f;
    if (!(v <= 127.0f)) v = 127.0f;
    lookAt->l[0x1A] = (s32) v;
    lookAt->l[0x00] = 0;
    lookAt->l[0x01] = 0;
    lookAt->l[0x02] = 0;
    lookAt->l[0x03] = 0;
    lookAt->l[0x04] = 0;
    lookAt->l[0x05] = 0;
    lookAt->l[0x06] = 0;
    lookAt->l[0x07] = 0;
    lookAt->l[0x10] = 0;
    lookAt->l[0x11] = 0x80;
    lookAt->l[0x12] = 0;
    lookAt->l[0x13] = 0;
    lookAt->l[0x14] = 0;
    lookAt->l[0x15] = 0x80;
    lookAt->l[0x16] = 0;
    lookAt->l[0x17] = 0;
    if (!gEngineApp_m_Game.m_Settings.multiplayer) {
        ang = fov * aspect * 0.008726647f;
        radius = cam->m_FarClip / func_002974C0(ang);
        corners[0].x = 0.0f;
        corners[0].y = 0.0f;
        corners[0].z = 0.0f;
        cornerX = radius * func_002982F0(ang);
        cornerY = cornerX / aspect;
        corners[1].x = cornerX;
        corners[1].y = cornerY;
        depth = -cam->m_FarClip;
        corners[2].x = -cornerX;
        corners[2].y = cornerY;
        corners[3].x = cornerX;
        corners[3].y = -cornerY;
        corners[4].x = -cornerX;
        corners[4].y = -cornerY;
        corners[1].z = depth;
        corners[2].z = depth;
        corners[3].z = depth;
        corners[4].z = depth;
    } else {
        boxX = halfX * 0.5f;
        boxY = halfY * 0.5f;
        corners[0].x = 0.0f;
        corners[0].y = 0.0f;
        corners[0].z = 0.0f;
        corners[1].x = boxX;
        corners[1].y = boxY;
        boxZ = -cam->m_FarClip;
        corners[2].x = -boxX;
        corners[2].y = boxY;
        corners[3].x = boxX;
        corners[3].y = -boxY;
        corners[4].x = -boxX;
        corners[4].y = -boxY;
        corners[1].z = boxZ;
        corners[2].z = boxZ;
        corners[3].z = boxZ;
        corners[4].z = boxZ;
        radius = func_00298470(boxX * boxX + boxY * boxY + boxZ * boxZ);
    }
    CMtxF__TransformPoints(cam->m_mfViewOrient, corners, cam->m_vTCorners, 5);
    Frustum_FromCorners(cam->m_ViewVolume, &cam->m_vTCorners[0], &cam->m_vTCorners[1], &cam->m_vTCorners[2],
                  &cam->m_vTCorners[3], &cam->m_vTCorners[4]);
    BoundingBox_FromPoints(&cam->m_ViewMin, 5, cam->m_vTCorners);
    r15 = radius * 1.5f;
    cam->m_AnimMin.x = cam->m_vTCorners[0].x - radius;
    cam->m_AnimMax.x = cam->m_vTCorners[0].x + radius;
    cam->m_AnimMin.y = cam->m_vTCorners[0].y - r15;
    cam->m_AnimMax.y = cam->m_vTCorners[0].y + r15;
    cam->m_AnimMin.z = cam->m_vTCorners[0].z - radius;
    cam->m_AnimMax.z = cam->m_vTCorners[0].z + radius;
    CQuaternion__ToMatrix(&cam->m_qViewAlignNoZ, cam->m_mfFaceScreen);
    CMtxF__SetRotateY(cam->m_mfFaceScreenYAxisUp, cam->m_vRotation.y);
    if (gEngineApp_m_Game_m_CameraPool.m_bActiveChanged != 0) {
        guPerspectiveF(mProj, &cam->m_OnScreenPerspNorm, 47.5f, gScreenWidth / gScreenHeight, 16.0f, 7168.0f, 1.0f);
        CMtxF__Identity(mView);
        CMtxF__Multiply(m, mView, mProj);
        CMtxF__SetScale(mFlip, -1.0f, 1.0f, 1.0f);
        CMtxF__Multiply(mOnScreen, m, mFlip);
        CMtxF__ClampEntries(mOnScreen);
        CMtxF__ToMtx(mOnScreen, &cam->m_mOnScreenProjection[gEvenOdd]);
    }
    CEmitterPool__Advance(cam->m_SkySystem, cam);
    MenuPool_Update(cam->m_MenuPool);
    if (Cinema_PathPlaying() != 0) {
        cam->m_ViewParams.m_pCurrentRegion = (struct CGameRegion *) CScene__FindRegionAt(&gScene, &cam->m_ViewParams.m_vPos);
    }
    floor = (CFloor *) cam->m_ViewParams.m_pCurrentRegion;
    x = cam->m_ViewParams.m_vPos.x;
    y = cam->m_ViewParams.m_vPos.y;
    z = cam->m_ViewParams.m_vPos.z;
    height = cam->m_ViewParams.m_YPosOffset;
    cam->m_ViewParams.m_VisBits = gDefaultVisBits;
    if (floor != NULL && Cinema_PathPlaying() == 0) {
        above = ((y + height) - CGameRegion__GetHeight((struct CGameRegion *) floor, x, z)) * 0.09765625f;
        if (above < 11.0f && above > 5.0f) {
            cam->m_ViewParams.m_VisBits = floor->m_VisBits;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        cam->m_ViewParams.m_VisBits = gDefaultVisBits;
    }
    if (cam->m_ViewParams.m_ZoomBlend > 0.0f) {
        cam->m_ViewParams.m_VisBits = gDefaultVisBits;
    }
}

void Texture_InvalidateCache(void);
void Render_SetRenderMode(s32 arg0);
u32 Cinema_GetFogColor(void);        /* a cinematic's fog colour, 0xFF00 for none */
void DLights_Load(CCamera *cam);
#define GFX(dl, a, b) GFX_W((dl)++, a, b)

static __inline__ void ResetPixelScale(CCamera *cam) {
    cam->m_PixelXScale = 1.0f;
    cam->m_PixelYScale = 1.0f;
}

/* The warp effect's scale, as an inline setter: its arguments are copied
 * into the parameters, and the scales are then read back from the camera
 * (reload turns the m_PixelXScale read into the ROM's mov.s). */
static __inline__ void SetPixelScale(CCamera *cam, f32 xs, f32 ys) {
    cam->m_PixelXScale = xs;
    cam->m_PixelYScale = ys;
}

/* Opens a camera's part of the frame's display list (LibTEngine's CCamera
 * DisplayListSetup): its projection, viewport, scissor and look-at; then,
 * unless it is the main camera with others active, clears the z buffer and
 * the view to the fog colour, with the fog from m_FogStart to 1000. */
void CCamera__DisplayListSetup(CCamera *cam) {
    struct { u8 red, green, blue; } rgb;
    s16 ulx, uly, lrx, lry;
    s32 near, far, denom;
    u32 color;
    u32 fill;
    u32 red, green, blue;
    s16 vx, vy;
    s32 maxx, maxy;
    f32 sx, sy, width, height, x, y;
    CFrameData *buffer;
    s32 frame;

    {
        Gfx **setup = &gDList;

        GFX(*setup, 0xE7000000, 0);                         /* gDPPipeSync */
        GFX(*setup, 0xDE000000, GBI_ADDR(gRspInitDL));          /* gSPDisplayList */
        GFX(*setup, 0xDE000000, GBI_ADDR(gRdpInitDL));
        ResetPixelScale(cam);
        SetPixelScale(cam, cam->m_WarpXScale, cam->m_WarpYScale);
        sx = cam->m_PixelXScale;
        sy = cam->m_PixelYScale;
        maxx = (u16) gScreenWidth - 1;
        maxy = (u16) gScreenHeight - 1;
        width = cam->m_Viewport.m_Width;
        height = cam->m_Viewport.m_Height;
        x = cam->m_Viewport.m_ScreenPosX;
        y = cam->m_Viewport.m_ScreenPosY;
        ulx = (s32) x;
        uly = (s32) y;
        vx = (s32) ((width + width) * sx);
        lrx = (s32) (x + width);
        lry = (s32) (y + height);
        vy = (s32) ((height + height) * sy);
        if (gScreenWidth - 1 < lrx) lrx = maxx;
        if (ulx < 0) ulx = 0;
        if (uly < 0) uly = 0;
        if (lry > gScreenHeight - 1) lry = maxy;
        buffer = gEngineApp.m_pCurrentFrameData;
        frame = gEvenOdd;
        GFX(*setup, 0xDA380007, GBI_ADDR(&cam->m_mProjection[frame]));   /* gSPMatrix */
        GFX(*setup, 0xDB0E0000, cam->m_PerspNorm);                    /* gSPPerspNormalize */
        cam->m_vp[frame].vscale[0] = vx;
        cam->m_vp[frame].vscale[1] = vy;
        cam->m_vp[frame].vtrans[0] = (s32) ((vx * 0.25f + cam->m_Viewport.m_ScreenPosX) * 4.0f);
        cam->m_vp[frame].vtrans[1] = (s32) ((vy * 0.25f + cam->m_Viewport.m_ScreenPosY) * 4.0f);
        GFX(*setup, 0xDC080008, GBI_ADDR(&cam->m_vp[frame]));             /* gSPViewport */
        GFX(*setup, 0xED000000 | ((s32) (ulx * 4.0f) & 0xFFF) << 12 | ((s32) (uly * 4.0f) & 0xFFF),
            ((s32) (lrx * 4.0f) & 0xFFF) << 12 | ((s32) (lry * 4.0f) & 0xFFF));   /* gDPSetScissor */
        GFX(*setup, 0xDC08000A, GBI_ADDR(&cam->m_LookAt[frame]));                        /* gSPLookAtX */
        GFX(*setup, 0xDC08030A, GBI_ADDR(&cam->m_LookAt[frame]) + 0x10);                 /* gSPLookAtY */
        Texture_InvalidateCache();
    }
    GFX(gDList, 0xE3001201, 0x2000);                    /* gDPSetTextureFilter(bilerp) */
    if (cam != &gEngineApp_m_Game_m_CameraPool.m_MainCamera || gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount == 0) {
        Gfx **clearDl = &gDList;

        Render_SetRenderMode(0x14);
        GFX(*clearDl, 0xE7000000, 0);
        GFX(*clearDl, 0xE3000A01, 0x300000);                /* gDPSetCycleType(G_CYC_FILL) */
        GFX(*clearDl, 0xFF100000 | ((gScreenWidth - 1) & 0xFFF), (uptr) gDepthBuffer);   /* colour image: z buffer */
        GFX(*clearDl, 0xE7000000, 0);
        GFX(*clearDl, 0xE3000A01, 0x300000);
        GFX(*clearDl, 0xF7000000, 0xFFFCFFFC);              /* gDPSetFillColor(max z) */
        {
            Gfx *cmd = *clearDl;

            GFX(cmd, 0xF6000000 | (lrx & 0x3FF) << 14 | (lry & 0x3FF) << 2,
                (ulx & 0x3FF) << 14 | (uly & 0x3FF) << 2);  /* gDPFillRectangle */
            *clearDl = cmd;
        }
        if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
            rgb.red = 0;
            rgb.green = 0;
            rgb.blue = 0;
        } else {
            rgb.red = cam->m_FogColor[0] & 0xF8;
            rgb.green = cam->m_FogColor[1] & 0xF8;
            rgb.blue = cam->m_FogColor[2] & 0xF8;
        }
        if (Cinema_PathPlaying() != 0) {
            color = Cinema_GetFogColor();
            if (color != 0xFF00) {
                rgb.blue = color >> 16;
                rgb.green = color >> 8;
                rgb.red = color;
            }
        }
        GFX(gDList, 0xD9FFFFFF, 0x10000);               /* gSPSetGeometryMode(G_FOG) */
        {
            Gfx *g = gDList++;                          /* gSPFogPosition(m_FogStart, 1000) */
            g->w0 = 0xDB080000;
            far = (u16) cam->m_FogStart;
            denom = 1000 - far;
            near = (500 - far) * 256;
            g->w1 = (u32) (0x1F400 / denom) << 16 | ((near / denom) & 0xFFFF);
        }
        {
            Gfx *g = gDList++;                          /* gDPSetFogColor */
            g->w0 = 0xF8000000;
            red = rgb.red;
            green = rgb.green;
            blue = rgb.blue;
            g->w1 = red << 24 | green << 16 | blue << 8 | 0xFF;
        }
        fill = ((red << 8) & 0xF800) | ((green << 3) & 0x7C0) | ((blue >> 2) & 0x3E);
        GFX(gDList, 0xFF100000 | ((gScreenWidth - 1) & 0xFFF), buffer->m_pFrameBuffer);
        GFX(gDList, 0xF7000000, ((fill | 1) << 16 | 1) + (u16) fill);
        GFX(gDList, 0xF6000000 | (lrx & 0x3FF) << 14 | (lry & 0x3FF) << 2,
            (ulx & 0x3FF) << 14 | (uly & 0x3FF) << 2);
    }
    GFX(gDList, 0xE7000000, 0);
    GFX(gDList, 0xE3001801, 0);                         /* gDPSetAlphaDither */
    GFX(gDList, 0xE3001A01, 0x10);
    GFX(gDList, 0xE3000A01, 0x100000);                  /* gDPSetCycleType(G_CYC_2CYCLE) */
    if (cam != &gEngineApp_m_Game_m_CameraPool.m_MainCamera) {
        DLights_Load(cam);
    }
    GFX(gDList, 0xE7000000, 0);
}

#include "globals/swoosh.h"
#ifdef TARGET_PORT
#include "structs/world.h"
#endif

void func_002847D0(void);
void CPlayerPool__ReloadAnimInfo(CPlayerPool *list, CCamera *cam);
void CPlayerPool__DrawCharacters(CPlayerPool *list, CCamera *cam);
void SwooshSystem_DrawDecals(void *arg0, CCamera *cam);
void CLevelActorPool__Draw(u8 *arg0, CCamera *cam);
void CEmitterPool__Draw(u8 *sky, CCamera *cam);
void Trail_DrawAll(void *arg0, CCamera *cam);
void CCamera__DrawGlares(CCamera *cam);
void CPlayerPool__DrawWeaponOverlays(CPlayerPool *list, CCamera *cam);

/* Draws the scene through a camera, layer by layer (LibTEngine's CCamera
 * DrawScene), and with game flag 0x200 up clears its viewport after: a
 * fill rectangle in fill mode, blend colour and prim depth all ones. */
void CCamera__DrawScene(CCamera *cam, u8 *world) {
    func_002847D0();
    CScene__DrawGeometry(world, cam);
    if (Cinema_PathPlaying() == 0) {
        CPlayerPool__ReloadAnimInfo(&gEngineApp_m_Game.m_PlayerList, cam);
    }
    if (gEngineApp_m_Game.m_Settings.mode == 13) {
        CPlayerPool__DrawCharacters(&gEngineApp_m_Game.m_PlayerList, cam);
    }
    CScene__DrawRegisteredInstances(world, cam);
    SwooshSystem_DrawDecals(&gEngineApp_m_SwooshSystem, cam);
    if (Cinema_PathPlaying() == 0) {
#ifdef TARGET_PORT
        CLevelActorPool__Draw((u8 *) &((CScene *) world)->m_ActorPool, cam);
#else
        CLevelActorPool__Draw(world + 0x10518, cam);
#endif
    }
    CScene__DrawSortedList(world, cam);
    CEmitterPool__Draw(cam->m_SkySystem, cam);
    CScene__DrawParticles(world, cam);
    Trail_DrawAll(&gEngineApp_m_SwooshSystem, cam);
    CCamera__DrawGlares(cam);
    if (Cinema_PathPlaying() == 0 && gEngineApp_m_Game.m_Settings.mode == 13) {
        CPlayerPool__DrawWeaponOverlays(&gEngineApp_m_Game.m_PlayerList, cam);
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x200) {
        GFX_CMD(0xE7000000, 0);                   /* gDPPipeSync */
        GFX_CMD(0xE3000A01, 0);                   /* gDPSetCycleType(G_CYC_FILL) */
        GFX_CMD(0xF9000000, 0xFFFFFFFF);          /* gDPSetBlendColor */
        GFX_CMD(0xEE000000, 0xFFFFFFFF);          /* gDPSetPrimDepth */
        GFX_CMD(0xE2001D00, 4);                   /* gDPSetRenderMode, z compare */
        Render_SetRenderMode(0x16);
        {                                           /* gDPFillRectangle over the viewport */
        Gfx *g = gDList++;
        g->w0 = 0xF6000000
              | ((u32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width) & 0x3FF) << 14
              | ((u32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height) & 0x3FF) << 2;
        g->w1 = ((u32) cam->m_Viewport.m_ScreenPosX & 0x3FF) << 14
              | ((u32) cam->m_Viewport.m_ScreenPosY & 0x3FF) << 2;
        }
    }
}

#define LINKS(T) T2_OFFSETOF(T, m_pLast), T2_OFFSETOF(T, m_pNext)
#define TEXTS_SIZE (16 * sizeof(CCameraText))

void Sound__SetListener(void *);
void CCache__Flush(s32);
void CCache__Deallocate(sptr, void *);
struct CMemEntry *CCache__Allocate(s32, s32, u32, char *);
void *memset(void *, s32, uptr);

VM_TEXT static __inline__ void clearAxis(CShakeAxis *axis) {
    axis->m_Shake = 0;
    axis->m_LastShake = 0;
    axis->m_Amp = 0;
    axis->m_Angle = 0;
}

VM_TEXT static __inline__ void initShake(CShake *shake, f32 radius) {
    shake->m_vPos.x = 0;
    shake->m_vPos.y = 0;
    shake->m_vPos.z = 0;
    shake->m_Radius = radius;
    clearAxis(&shake->m_XAxis);
    clearAxis(&shake->m_YAxis);
    clearAxis(&shake->m_ZAxis);
}

VM_TEXT void CCameraPool__NewGame(CCameraPool *owner, s32 count) {
    s32 i;
    f32 value;

    owner->m_ActiveLastFrame = 0;
    owner->m_bActiveChanged = 0;
    owner->m_MainCamera.m_Index = 0;
    CCamera__Construct(&owner->m_MainCamera);
    owner->m_bActiveChanged = 2;
    CList__Construct(&owner->m_ShakeFreeList, LINKS(CShake));
    CList__Construct(&owner->m_ShakeActiveList, LINKS(CShake));
    value = 1.0f;
    for (i = 0; i < 8; i++) {
        initShake(&owner->m_ShakeVars[i], value);
        CList__AddTail(&owner->m_ShakeFreeList, &owner->m_ShakeVars[i]);
    }
    CList__Construct(&owner->m_FreeList, LINKS(CCamera));
    CList__Construct(&owner->m_ActiveList, LINKS(CCamera));
    CList__Construct(&owner->m_FreeTextList, LINKS(CCameraText));
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
    owner->m_pmeTexts = CCache__Allocate(0, TEXTS_SIZE, 0x23, "Camera Texts");
    owner->m_Texts = *(CCameraText **) owner->m_pmeTexts;
    memset(owner->m_Texts, 0, TEXTS_SIZE);
    for (i = 0; i < owner->m_nTexts; i++) {
        CList__AddTail(&owner->m_FreeTextList, &owner->m_Texts[i]);
    }
}

s32 SavedPlayers_IsActive(void);
/* One step of a viewport value toward its target: 16 pixels a frame, or
 * there at once when SavedPlayers_IsActive says so; while it moves, gScreenClearTime is
 * set to 30. */
#define STEP_TOWARD(field, target)              \
    {                                           \
        f32 v = (field);                        \
        f32 speed = 16.0f;                      \
        if (SavedPlayers_IsActive()) {                  \
            v = (target);                       \
        }                                       \
        if (v < (target)) {                     \
            v += speed;                         \
            gScreenClearTime = 30.0f;                 \
            if ((target) < v) {                 \
                v = (target);                   \
            }                                   \
        } else if ((target) < v) {              \
            v -= speed;                         \
            gScreenClearTime = 30.0f;                 \
            if (v < (target)) {                 \
                v = (target);                   \
            }                                   \
        }                                       \
        (field) = v;                            \
    }

/* Moves the camera's viewport toward a size and a place on the screen (a
 * split screen opening or closing). */
void CCamera__MoveViewport(CCamera *cam, f32 width, f32 height, f32 x, f32 y) {
    STEP_TOWARD(cam->m_Viewport.m_Width, width);
    STEP_TOWARD(cam->m_Viewport.m_Height, height);
    STEP_TOWARD(cam->m_Viewport.m_ScreenPosX, x);
    STEP_TOWARD(cam->m_Viewport.m_ScreenPosY, y);
}

#include "structs/collision.h"
/* gDefaultVisBits read through the same view: as a plain u32 GCC 2 loads it before the box copies (alias order). */
#define DEFAULT_VIS_BITS (((CVisBits *) &gDefaultVisBits)->m_Bits)
/* A camera's m_pNext, read as a plain word (not as a structure field, which
 * would let GCC move the read above the store to gCameraVisBits before it). */
#ifdef TARGET_PORT
/* the host CCamera: m_pNext after the 8-byte m_pLast (soak S2: a second active camera crashed) */
#define NEXT_CAMERA(c) ((c)->m_pNext)
#else
#define NEXT_CAMERA(c) (*(CCamera **) ((u8 *) (c) + 4))
#endif

/* Advances the camera pool (LibTEngine's CCameraPool Advance): returns the
 * finished shakes to the free list, lays out the viewports of the active
 * cameras for one to four players, advances the main camera and each active
 * one, gathering their view boxes and visibility bits, and counts down
 * m_bActiveChanged after the number of active cameras changes. */
void CCameraPool__Advance(CCameraPool *pool) {
    f32 halfWidth, halfHeight, width, height;
    f32 top, left, right, bottom;
    f32 splitX, splitY;
    CShake *shake;
    CShake *nextShake;
    u32 xDone, yDone, zDone;
    s32 finished;
    s32 timer;
    CCamera *view;
    CBox *clip;
    CBox *bounds;
    CVisBits *flags;
    f32 v;

    right = (f32) gScreenWidth;
    left = 0.0f;
    bottom = (f32) gScreenHeight;
    top = 0.0f;
    if ((u32) pool->m_ActiveList.m_nCount >= 2U) {
        top = gSplitScreenMarginTop;
        bottom -= gSplitScreenMarginBottom;
        left = gSplitScreenMarginLeft;
        right -= gSplitScreenMarginRight;
    }
    width = right - left;
    halfWidth = width * 0.5f;
    height = bottom - top;
    halfHeight = height * 0.5f;
    shake = (CShake *) gEngineApp_m_Game_m_CameraPool.m_ShakeActiveList.m_pHead;
    view = &pool->m_MainCamera;
    if (shake != NULL) {
        do {
            nextShake = shake->m_pNext;
            xDone = CShakeAxis__Advance(&shake->m_XAxis);
            yDone = CShakeAxis__Advance(&shake->m_YAxis);
            zDone = CShakeAxis__Advance(&shake->m_ZAxis);
            finished = 0;
            if (xDone != 0 && yDone != 0) {
                finished = zDone > 0U;
            }
            if (finished != 0) {
                CList__Remove(&pool->m_ShakeActiveList, shake);
                CList__AddHead(&pool->m_ShakeFreeList, shake);
            }
            shake = nextShake;
            view = &pool->m_MainCamera;
        } while (shake != NULL);
    }
    view->m_Viewport.m_Width = (f32) gScreenWidth;
    view->m_Viewport.m_Height = (f32) gScreenHeight;
    view->m_Viewport.m_ScreenPosX = 0;
    view->m_Viewport.m_ScreenPosY = 0;
    view = (CCamera *) pool->m_ActiveList.m_pHead;
    switch ((u32) pool->m_ActiveList.m_nCount) {
    case 1:
        CCamera__MoveViewport(view, width, height, 0.0f, 0.0f);
        break;
    case 2:
        if (gEngineApp_m_Settings.unk1B == 0) {
            CCamera__MoveViewport(view, width, halfHeight, left, top);
            view = view->m_pNext;
            CCamera__MoveViewport(view, width, halfHeight, left, top + halfHeight);
        } else {
            CCamera__MoveViewport(view, halfWidth, height, left, top);
            view = view->m_pNext;
            CCamera__MoveViewport(view, halfWidth, height, left + halfWidth, top);
        }
        break;
    case 3:
        CCamera__MoveViewport(view, halfWidth, halfHeight, left, top);
        view = view->m_pNext;
        CCamera__MoveViewport(view, halfWidth, halfHeight, left + halfWidth, top);
        view = view->m_pNext;
        CCamera__MoveViewport(view, halfWidth, halfHeight, left, top + halfHeight);
        break;
    case 4:
        CCamera__MoveViewport(view, halfWidth, halfHeight, left, top);
        view = view->m_pNext;
        splitX = left + halfWidth;
        CCamera__MoveViewport(view, halfWidth, halfHeight, splitX, top);
        view = view->m_pNext;
        splitY = top + halfHeight;
        CCamera__MoveViewport(view, halfWidth, halfHeight, left, splitY);
        view = view->m_pNext;
        CCamera__MoveViewport(view, halfWidth, halfHeight, splitX, splitY);
        break;
    }
    clip = &gCameraViewBox;
    *clip = *(CBox *) &pool->m_MainCamera.m_ViewMin;
    bounds = &gCameraVisibleBox;
    *bounds = *(CBox *) &pool->m_MainCamera.m_AnimMin;
    gCameraVisBits = DEFAULT_VIS_BITS;
    CCamera__Advance(&pool->m_MainCamera);
    view = (CCamera *) pool->m_ActiveList.m_pHead;
    if (view != NULL) {
        CCamera__Advance(view);
        *clip = *(CBox *) &view->m_ViewMin;
        *bounds = *(CBox *) &view->m_AnimMin;
        gCameraVisBits = view->m_ViewParams.m_VisBits;
        view = NEXT_CAMERA(view);
        if (view != NULL) {
            flags = (CVisBits *) &gCameraVisBits;
            do {
                CCamera__Advance(view);
                v = view->m_ViewMin.x;
                if (!(v <= clip->m_vMin.x)) v = clip->m_vMin.x;
                clip->m_vMin.x = v;
                v = view->m_ViewMin.y;
                if (!(v <= clip->m_vMin.y)) v = clip->m_vMin.y;
                clip->m_vMin.y = v;
                v = view->m_ViewMin.z;
                if (!(v <= clip->m_vMin.z)) v = clip->m_vMin.z;
                clip->m_vMin.z = v;
                v = view->m_ViewMax.x;
                if (!(clip->m_vMax.x <= v)) v = clip->m_vMax.x;
                clip->m_vMax.x = v;
                v = view->m_ViewMax.y;
                if (!(clip->m_vMax.y <= v)) v = clip->m_vMax.y;
                clip->m_vMax.y = v;
                v = view->m_ViewMax.z;
                if (!(clip->m_vMax.z <= v)) v = clip->m_vMax.z;
                clip->m_vMax.z = v;
                v = view->m_AnimMin.x;
                if (!(v <= bounds->m_vMin.x)) v = bounds->m_vMin.x;
                bounds->m_vMin.x = v;
                v = view->m_AnimMin.y;
                if (!(v <= bounds->m_vMin.y)) v = bounds->m_vMin.y;
                bounds->m_vMin.y = v;
                v = view->m_AnimMin.z;
                if (!(v <= bounds->m_vMin.z)) v = bounds->m_vMin.z;
                bounds->m_vMin.z = v;
                v = view->m_AnimMax.x;
                if (!(bounds->m_vMax.x <= v)) v = bounds->m_vMax.x;
                bounds->m_vMax.x = v;
                v = view->m_AnimMax.y;
                if (!(bounds->m_vMax.y <= v)) v = bounds->m_vMax.y;
                bounds->m_vMax.y = v;
                v = view->m_AnimMax.z;
                if (!(bounds->m_vMax.z <= v)) v = bounds->m_vMax.z;
                bounds->m_vMax.z = v;
                flags->m_Bits |= view->m_ViewParams.m_VisBits;
                view = view->m_pNext;
            } while (view != NULL);
        }
    }
    if (pool->m_ActiveLastFrame != pool->m_ActiveList.m_nCount) {
        pool->m_bActiveChanged = 2;
        pool->m_ActiveLastFrame = pool->m_ActiveList.m_nCount;
        return;
    }
    timer = pool->m_bActiveChanged;
    if (timer != 0) {
        pool->m_bActiveChanged = timer - 1;
    }
}

#include "structs/frame.h"

void CCache__LockForDisplayList(s32, struct CMemEntry *);
void Render_InvalidateModeCache(void);
void CCameraPool__AdvanceText(CCameraPool *pool, CCamera *cam);
void MenuPool_Draw(void *menuPool);
void CCamera__DrawBorder(CCamera *cam);
void CPlayerPool__DrawMatchClock(CPlayerPool *list, CCamera *cam);

/* Updates every camera of the pool, then the main camera. While
 * gScreenClearTime counts down, the frame is first cleared to black. */
void CCameraPool__Draw(CCameraPool *pool, void *world) {
    CCamera *cam;
    CCamera *main;
    s32 w;
    s32 h;

    pool->m_MainCamera.m_FogColor[0] = 0;
    pool->m_MainCamera.m_FogColor[1] = 0;
    pool->m_MainCamera.m_FogColor[2] = 0;
    if (pool->m_pmeCameras != NULL) {
        CCache__LockForDisplayList(0, pool->m_pmeCameras);
    }
    if (gEngineApp_m_Settings.paused != 0 || pool->m_ActiveList.m_nCount == 3) {
        gScreenClearTime = 30.0f;
    }
    if (gScreenClearTime != 0.0f) {
        gScreenClearTime -= gFrameIncrement;
    }
    if (gScreenClearTime < 0.0f) {
        gScreenClearTime = 0.0f;
    }
    if (gScreenClearTime != 0.0f) {
        cam = pool->m_ActiveList.m_pHead;
        if (cam != NULL) {
            GFX_CMD(0xE7000000, 0)
            GFX_CMD(0xDE000000, GBI_ADDR(gRspInitDL))
            GFX_CMD(0xDE000000, GBI_ADDR(gRdpInitDL))
            cam->m_vp[gEvenOdd].vscale[0] = gScreenWidth;
            cam->m_vp[gEvenOdd].vscale[1] = gScreenHeight;
            cam->m_vp[gEvenOdd].vtrans[0] = gScreenWidth;
            cam->m_vp[gEvenOdd].vtrans[1] = gScreenHeight;
            GFX_CMD(0xDC080008, GBI_ADDR(&cam->m_vp[gEvenOdd]))
            w = gScreenWidth;
            h = gScreenHeight;
            GFX_CMD(0xED000000, ((((s32)((f32)(w - 1) * 4.0f)) & 0xFFF) << 12) | (((s32)((f32)(h - 1) * 4.0f)) & 0xFFF))
            GFX_CMD(0xFF100000 | ((w - 1) & 0xFFF), gEngineApp_m_pCurrentFrameData->m_pFrameBuffer)
            GFX_CMD(0xE3000A01, 0x300000)
            GFX_CMD(0xFCFFFFFF, 0xFFFE793C)
            GFX_CMD(0xE200001C, 0)
            GFX_CMD(0xF7000000, 0x10001)
            GFX_CMD(0xF6000000 | ((w & 0x3FF) << 14) | ((h & 0x3FF) << 2), 0)
            Render_InvalidateModeCache();
        }
    }
    for (cam = pool->m_ActiveList.m_pHead; cam != NULL; cam = cam->m_pNext) {
        CCamera__DisplayListSetup(cam);
        if (gCameraPoolHideViews == 0) {
            CCamera__DrawScene(cam, world);
        }
        CCamera__DrawTints(cam);
        CCameraPool__AdvanceText(pool, cam);
        MenuPool_Draw(cam->m_MenuPool);
        CCamera__DrawBorder(cam);
    }
    main = &pool->m_MainCamera;
    CCamera__DisplayListSetup(main);
    if (pool->m_ActiveList.m_nCount == 0) {
        CCamera__DrawTints(main);
    }
    CCameraPool__AdvanceText(pool, main);
    if (gEngineApp_m_Game.m_Settings.multiplayer != 0) {
        cam = pool->m_ActiveList.m_pHead;
        if (cam != NULL) {
            CPlayerPool__DrawMatchClock(&gEngineApp_m_Game.m_PlayerList, cam);
        }
    }
    MenuPool_Draw(pool->m_MainCamera.m_MenuPool);
}

/* gDPFillRectangle with float corners, each converted to an unsigned int. */
#define FILL_RECT(ulx, uly, lrx, lry)                                               \
    {                                                                               \
        Gfx *_g = gDList++;                                                     \
        _g->w0 = 0xF6000000 | (T2_F2U(lrx) & 0x3FF) << 14 | (T2_F2U(lry) & 0x3FF) << 2;   \
        _g->w1 = (T2_F2U(ulx) & 0x3FF) << 14 | (T2_F2U(uly) & 0x3FF) << 2;          \
    }

/* Draws a one-pixel frame around a camera's viewport (the borders of a
 * split screen): four fill rectangles in fill mode, colour 0x10001. */
void CCamera__DrawBorder(CCamera *cam) {
    f32 x, y, width, height;

    Render_InvalidateModeCache();
    x = cam->m_Viewport.m_ScreenPosX;
    y = cam->m_Viewport.m_ScreenPosY;
    width = cam->m_Viewport.m_Width;
    height = cam->m_Viewport.m_Height;
    GFX_CMD(0xE7000000, 0);                     /* gDPPipeSync */
    GFX_CMD(0xE3000A01, 0x300000);              /* gDPSetCycleType(G_CYC_FILL) */
    GFX_CMD(0xFCFFFFFF, 0xFFFE793C);            /* gDPSetCombineMode */
    GFX_CMD(0xE200001C, 0);                     /* gDPSetRenderMode */
    GFX_CMD(0xF7000000, 0x10001);               /* gDPSetFillColor */
    FILL_RECT(x, y, x + width - 1.0f, y);
    FILL_RECT(x, y + height - 1.0f, x + width - 1.0f, y + height - 1.0f);
    FILL_RECT(x, y, x, y + height - 1.0f);
    FILL_RECT(x + width - 1.0f, y, x + width, y + height - 1.0f);
}

#include "globals/strings.h"
#include "globals/hud.h"

void CCameraText__Reset(CCameraText *arg0);

/* CCameraPool__TakeText, inlined: takes a free text from the pool and puts it on the
 * camera's active texts. */
static __inline__ CCameraText *TakeText(CCameraPool *pool, CCamera *cam) {
    CCameraText *text = pool->m_FreeTextList.m_pHead;

    if (text != NULL) {
        CCameraText__Reset(text);
        CList__Remove(&pool->m_FreeTextList, text);
        CList__AddTail(&cam->m_ActiveTextList, text);
    }
    return text;
}

s32 Cinema_IsActive(void);

/* Shows a string over a camera for a second, one text per line, centred, 80
 * pixels above the bottom, with no id (-1). When Cinema_IsActive() says so, the
 * texts the camera shows are put in mode 4 and the new ones start in mode 1
 * instead of 2. Returns the last line's text. */
CCameraText *CCameraPool__AddText(CCameraPool *pool, CCamera *cam, u8 *string) {
    CCameraText *cur;
    CCameraText *text;
    u8 *str;
    u8 *line;
    u8 *c;
    s32 mode;
    s32 id;

    mode = 2;
    if (Cinema_IsActive()) {
        cur = cam->m_ActiveTextList.m_pHead;
        if (cur != NULL) {
            mode = 1;
            do {
                cur->m_nMode = 4;
                cur = cur->m_pNext;
            } while (cur != NULL);
        }
    }
    id = -1;
    str = string;
    text = NULL;
    if (str == *(u8 **) sCinemaTextFadeOut) {
        str = sCameraDefaultText;
    }
    c = str;
    line = c;
    while (*c != 0) {
        c++;
        if (*c == '\n' || *c == 0) {
            if (line != c) {
                text = TakeText(pool, cam);
                if (text != NULL) {
                    text->m_nMode = mode;
                    text->m_pString = (char *)line;
                    text->m_nAlpha = 0;
                    text->m_Scale = 1.0f;
                    text->m_DisplayTime = 15.0f;
                    text->m_nId = id;
                    text->m_bSmall = 0;
                    text->m_ModeTime = 0.0f;
                    text->m_ModeDuration = 0.0f;
                    text->m_ScaleX = 1.0f;
                    text->m_ScaleY = 1.0f;
                    text->m_PositionX = gScreenWidth / 2;
                    text->m_PositionY = gScreenHeight - 80;
                }
                line = c + 1;
            }
        }
    }
    return text;
}

void COnScreen__BeginText(void);
void COnScreen__SelectTinyFont(void);
void COnScreen__DrawString(char *string, s32 x, s32 y, s32 alpha, s32 arg4, s32 arg5, f32 scaleX, f32 scaleY);

/* A camera's on-screen texts, each frame (LibTEngine's CCameraPool
 * AdvanceText): stacked 9 pixels apart from 81% down the screen, drawn
 * last to first, faded in (mode 2) and out (mode 4) over a fifteenth of a
 * second per unit of time with a quarter sine wave; when one has faded out
 * the waiting ones (mode 1) start in. Finished texts go back to the pool. */
void CCameraPool__AdvanceCinemaText(CCameraPool *pool, CCamera *cam) {
    CCameraText *text;
    CCameraText *cur;
    CCameraText *prev;
    CCameraText *other;
    f32 f;                      /* the gap between lines, then the time step */
    f32 wave;

    COnScreen__BeginText();
    COnScreen__SelectTinyFont();
    f = 9.0f;
    prev = cam->m_ActiveTextList.m_pHead;
    if (prev != NULL) {
        prev->m_PositionY = gScreenHeight * 0.81f;
        for (text = prev->m_pNext; text != NULL; text = text->m_pNext) {
            text->m_PositionY = prev->m_PositionY + f;
            prev = text;
        }
    }
    for (text = cam->m_ActiveTextList.m_pTail; text != NULL; text = text->m_pLast) {
        COnScreen__DrawString(text->m_pString, text->m_PositionX, text->m_PositionY, text->m_nAlpha, 1, 1, 1.0f, 1.0f);
    }
    for (text = cam->m_ActiveTextList.m_pTail; text != NULL; text = text->m_pLast) {
        f = gFrameIncrement * (1.0f / 15.0f);
        cur = text;
        switch (text->m_nMode) {
            case 1:                 /* waiting */
                text->m_nAlpha = 0;
                break;
            case 2:                 /* fading in */
                wave = func_002982F0(cur->m_ModeTime * 1.5707964f);
                cur->m_ModeTime += f;
                cur->m_nAlpha = wave * 255.0f;
                if (cur->m_ModeTime > 1.0f) {
                    cur->m_nMode = 3;
                    cur->m_ModeTime = 0.0f;
                }
                break;
            case 3:                 /* shown */
                cur->m_nAlpha = 255;
                break;
            case 4:                 /* fading out: the waiting ones start after */
                wave = func_002974C0(cur->m_ModeTime * 1.5707964f);
                cur->m_ModeTime += f;
                cur->m_nAlpha = wave * 255.0f;
                if (cur->m_ModeTime > 1.0f) {
                    for (other = cam->m_ActiveTextList.m_pHead; other != NULL; other = other->m_pNext) {
                        if (other->m_nMode == 1) {
                            other->m_nMode = 2;
                            other->m_ModeTime = 0.0f;
                        }
                    }
                    cur->m_nMode = 0;
                    cur->m_ModeTime = 0.0f;
                }
                break;
            default:
                cur->m_nAlpha = 0;
                cur->m_nMode = 0;
                break;
        }
    }
    for (text = cam->m_ActiveTextList.m_pTail; text != NULL; text = prev) {
        prev = text->m_pLast;
        if (text->m_nMode == 0) {
            CList__Remove(&cam->m_ActiveTextList, text);
            CList__AddHead(&pool->m_FreeTextList, text);
        }
    }
}

void COnScreen__SelectSmallFont(void);

/* A camera's on-screen texts, each frame (outside a cinema path; there
 * CCameraPool__AdvanceCinemaText does it): stacked upwards from 80% down the camera's
 * viewport, centred, drawn half size unless the viewport is smaller than the
 * screen or the text is small, last to first. A text grows in (mode 2), stays
 * its display time (mode 3) and shrinks away (mode 4); finished texts go back
 * to the pool. */
void CCameraPool__AdvanceText(CCameraPool *pool, CCamera *cam) {
    CCameraText *text;
    CCameraText *next;
    f32 xScale;
    f32 yScale;
    f32 size;
    f32 width;
    f32 height;
    f32 left;
    f32 top;
    f32 y;
    f32 x;
    s32 small;

    if (Cinema_PathPlaying()) {
        CCameraPool__AdvanceCinemaText(pool, cam);
        return;
    }
    size = 1.0f;
    height = cam->m_Viewport.m_Height;
    width = cam->m_Viewport.m_Width;
    left = cam->m_Viewport.m_ScreenPosX;
    top = cam->m_Viewport.m_ScreenPosY;
    yScale = height / gScreenHeight;
    xScale = width / gScreenWidth;
    small = 1;
    if (!(xScale < size) && !(yScale < size)) {
        small = 0;
    }
    x = width * 0.5f + left;
    y = height * 0.8f + top - yScale * 18.0f;
    for (text = cam->m_ActiveTextList.m_pTail; text != NULL; text = next) {
        next = text->m_pLast;
        if (text->m_nMode != 0) {
            COnScreen__BeginText();
            if (small || text->m_bSmall) {
                size = 1.0f;
                yScale = size;
                COnScreen__SelectSmallFont();
            } else {
                size = 0.5f;
            }
            COnScreen__DrawString(text->m_pString, x, y, text->m_nAlpha, 1, 1,
                          text->m_Scale * xScale * size, text->m_Scale * yScale * size);
            switch (text->m_nMode) {
                case 2:
                    text->m_nAlpha += 31;
                    if (text->m_nAlpha >= 255) {
                        text->m_nAlpha = 255;
                        text->m_nMode = 3;
                        text->m_Time = text->m_DisplayTime;
                    }
                    text->m_Scale = func_002982F0(text->m_nAlpha * 0.006159986f);
                    break;
                case 3:
                    text->m_nAlpha = 255;
                    text->m_Scale = 1.0f;
                    text->m_Time -= gFrameIncrement;
                    if (text->m_Time < 0.0f) {
                        text->m_nMode = 4;
                        text->m_nAlpha = 128;
                    }
                    break;
                case 4:
                    text->m_Scale -= func_002982F0(255.0f - text->m_nAlpha * 0.012319972f) * (1.0f / 3);
                    if (text->m_Scale < 0.0f) {
                        text->m_Scale = 0.0f;
                    }
                    text->m_nAlpha -= 12;
                    if (text->m_nAlpha < 0) {
                        text->m_nAlpha = 0;
                        text->m_nMode = 0;
                    }
                    break;
            }
            if (text->m_nMode != 0) {
                goto next_line;
            }
        }
        CList__Remove(&cam->m_ActiveTextList, text);
        CList__AddHead(&pool->m_FreeTextList, text);
    next_line:
        y -= yScale * 9.0f / size;
    }
}

void Hud_DrawSprite(s32 sprite, s32 frame, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 arg6);

/* A lens flare from the screen point (x, y): seven glares strung from it
 * through the centre of the screen, fading as the point leaves the centre.
 * With `sun` set the first glare is the sun itself (sprite 0x258), faded in
 * 35.2 pixels from the screen edges, and the flare does not fade with
 * distance. */
void CCamera__DrawGlare(CCamera *cam, f32 x, f32 y, f32 alpha, s32 sun, s32 unused) {
    f32 sizes[7] = { 16.0f, 16.0f, 24.0f, 32.0f, 16.0f, 32.0f, 48.0f };
    s32 opacities[7] = { 24, 24, 24, 24, 24, 24, 24 };
    s32 colours[21] = {
        255, 150, 150, 255, 255, 150, 150, 255, 150, 150, 255, 255,
        150, 150, 255, 200, 150, 255, 255, 150, 255
    };
    f32 dx, dy, dist, dirX, dirY, t, size, radius, adx, clamped;
    Gfx *g;
    s32 i;

    if (alpha != 0.0f) {
        dx = (f32) (gScreenWidth / 2) - x;
        dy = (f32) (gScreenHeight / 2) - y;
        {
            Gfx *g = gDList++;
            g->w0 = 0xE3001201;             /* gDPSetTextureFilter(bilerp) */
            g->w1 = 0x2000;
        }
        dist = func_00298470(dx * dx + dy * dy);
        if (sun == 0) {
            t = dist / func_00298470((f32) (gScreenWidth * gScreenWidth) * 0.25f + (f32) (gScreenHeight * gScreenHeight) * 0.25f);
            if (1.0f < t) {
                alpha = 0.0f;
            } else {
                alpha = alpha * (1.0f - t);
            }
        }
        dist = dist * 0.4f;
        if (dx < 0.0f) adx = -dx; else adx = dx;
        if (dy < 0.0f ? (-dy < adx) : (dy < adx)) {
            dirX = -1.0f;
            if (0.0f < dx) dirX = 1.0f;
            dirY = (dx < 0.0f) ? dy / -dx : dy / dx;
        } else {
            dirX = (dy < 0.0f) ? dx / -dy : dx / dy;
            dirY = -1.0f;
            if (0.0f < dy) dirY = 1.0f;
        }
        i = 0;
        if (sun) {
            f32 border, a, b, c, e, m, m2;

            border = 35.2f;
            a = (f32) gScreenHeight + border;
            b = (f32) gScreenWidth + border;
            c = x + border;
            e = y + border;
            m = a - y;
            if (!(m <= b - x)) m = b - x;
            if (!(m <= e)) m = e;
            radius = border;        /* the ROM keeps the border in radius's register */
            if (!(m <= c)) m = c;
            if (!(m <= radius)) m = radius;
            if (m < 0.0f) {
                alpha = 0.0f;
            } else {
                m2 = a - y;
                if (!(m2 <= b - x)) m2 = b - x;
                if (!(m2 <= e)) m2 = e;
                if (!(m2 <= c)) m2 = c;
                if (!(m2 <= radius)) m2 = radius;
                alpha = alpha * (m2 / border);
            }
            if (!(1.0f < alpha)) {
                clamped = 0.0f;
                if (!(alpha < 0.0f)) {
                    goto clamp;
                }
            } else {
            clamp:
                clamped = alpha;
                if (1.0f < clamped) clamped = 1.0f;
            }
            alpha = clamped;
            i = 0;
        }
        i = 0;
        do {
            x += dirX * dist;
            y += dirY * dist;
            if (sun != 0 && i == 0) {
                size = 2.0f;
            } else {
                size = sizes[i] * 0.03125f;
            }
            t = size * 0.55f;
            radius = t * 16.0f;
            if (!(-radius < x && x < (f32) gScreenWidth + radius && -radius < y && y < (f32) gScreenHeight + radius)) {
                return;
            }
            if (sun && i == 0) {
                g = gDList++;
                g->w0 = 0xFB000000;                     /* gDPSetEnvColor */
                g->w1 = (u32) (alpha * 255.0f) | 0xFFFFFF00;
                Hud_DrawSprite(0x258, 0, (s32) (x - radius), (s32) (y - radius), t, t, 1);
            } else {
                g = gDList++;
                g->w0 = 0xFB000000;
                g->w1 = (u8) colours[i * 3] << 24 | (u8) colours[i * 3 + 1] << 16 | (u8) colours[i * 3 + 2] << 8
                      | ((u32) (opacities[i] * alpha) & 0xFF);
                Hud_DrawSprite(0x259, 0, (s32) (x - radius), (s32) (y - radius), t, t, 1);
            }
            i++;
        } while (i < 7);
    }
}

#include "structs/particle.h"

/* The glares are particles whose template flags have 0x80000; m_dwFlags is the camera mask. */
void Hud_SetupTextureMode(s32 a);
void Render_SetRenderMode(s32 a);
void CMtxF__TransformPointProjective(CMtxF *m, CVector3 *in, CVector3 *out);

/* CCamera__WorldToScreen, inlined here. */
static inline void ScreenPos(CCamera *cam, CVector3 *point, f32 *x, f32 *y) {
    CVector3 value;
    f32 half;

    CMtxF__TransformPointProjective(&cam->m_mfProjection, point, &value);
    half = (f32) (gScreenWidth / 2);
    *x = value.x * half + half;
    *y = value.y * (f32) (-gScreenHeight / 2) + (f32) (gScreenHeight / 2);
}

/* The glares (lens flares) of the objects this camera sees. */
void CCamera__DrawGlares(CCamera *cam) {
    CParticle *e;
    u32 *dl;
    u32 *q0;
    u32 **pp;
    f32 sx, sy;
    s32 mask;

    mask = 8 << cam->m_Index;
    Hud_SetupTextureMode(0x64);
    Render_SetRenderMode(0x1A);
#ifdef TARGET_PORT /* one host Gfx (the N64 code writes two u32 words and steps 8 bytes) */
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3001201; /* gDPSetTextureFilter(G_TF_BILERP) */
        g->w1 = 0x2000;
    }
#else
    pp = (u32 **) &gDList;
    q0 = *pp;
    dl = q0;
    __asm__("" : "+r"(dl));
    dl[0] = 0xE3001201;
    dl[1] = 0x2000;
    __asm__("" : : : "memory");
    *pp = q0 + 2;
#endif
    for (e = gScene.m_ParticleSystem.m_DrawList.m_pHead; e != NULL; e = e->m_pDrawNext) {
        if ((e->m_dwFlags & mask) && (e->m_pEffect->m_dwFlags & 0x80000)) {
            ScreenPos(cam, &e->header.m_vPos, &sx, &sy);
            CCamera__DrawGlare(cam, sx, sy, e->m_Alpha * 0.003921569f, 0, 0);
        }
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00238EA8.c, func_00238EA8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
/* The port: the same stores through CCameraViewParams (structs/camera.h); the 4-byte slot
 * ObjInit has at 0x34 is the m_pCurrentRegion pointer, 8 bytes on the host. */
#include "common.h"
#include "globals/camera.h"
#include "structs/camera.h"

void CCameraViewParams__Construct(CCameraViewParams *o) {
    o->m_YPosOffset = 0.0f;
    o->m_nType = 1;
    o->m_RotY = 0.0f;
    o->m_vRotOffset.x = o->m_vRotOffset.y = o->m_vRotOffset.z = 0.0f;
    o->m_vPos.x = o->m_vPos.y = o->m_vPos.z = 0.0f;
    o->m_qGround.x = o->m_qGround.y = o->m_qGround.z = 0.0f;
    o->m_qGround.w = 1.0f;
    o->m_pCurrentRegion = NULL;
    o->m_ZoomBlend = 0.0f;
    o->m_ZoomFov = 0.17453295f;
    o->m_VisBits = gDefaultVisBits;
}
#else
#include "rw/junk_types.h"
#include "globals/camera.h"

#define DEFAULT_VIS_BITS (((CVisBits *) &gDefaultVisBits)->m_Bits)
void CCameraViewParams__Construct(CCameraViewParams *params) {
    ObjInit *o = (ObjInit *) params;
    CVector3 *p;
    f32 z;
    o->unk20.i = 0;
    z = o->unk20.f;
    o->unk0 = 1;
    o->unk4 = 0;
    o->unk8 = 0;
    o->unkC = 0;
    o->unk10 = 0;
    o->unk14 = 0;
    o->unk18 = 0;
    o->unk1C = 0;
    p = &o->v;
    p->x = p->y = p->z = z;
    o->unk30 = 1.0f;
    o->unk34 = 0;
    o->unk38 = z;
    o->unk3C = 0.17453295f;
    o->unk40 = DEFAULT_VIS_BITS;
}
#endif
#include "structs/world.h"

extern s32 Cinema_PathPlaying(void);
#ifdef TARGET_PORT
#include "structs/camera.h"
#include "structs/region.h"

f32 CGameRegion__GetHeight(struct CGameRegion *, f32, f32);

/* Host body on CCamera.m_ViewParams: the Rage Wars GameplayCamera view sits at N64 offsets (its
 * next pointer and the region slot are 4 bytes there), and its Shared_Quad (an s32/f32 union)
 * is passed in integer registers on x86-64 while the caller passes a CQuaternion in SSE
 * registers: every argument after it arrived wrong. */
void CCamera__SetViewParams(CCamera *cam, s32 type, f32 rotY, f32 rx, f32 ry, f32 rz, f32 x, f32 y,
                            f32 z, f32 yOffset, CQuaternion ground, CGameRegion *region, f32 zoomBlend,
                            f32 zoomFov) {
    CCameraViewParams *v = &cam->m_ViewParams;
    f32 value;

    v->m_nType = type;
    v->m_RotY = rotY;
    v->m_vRotOffset.x = rx;
    v->m_vRotOffset.y = ry;
    v->m_vRotOffset.z = rz;
    v->m_vPos.x = x;
    v->m_vPos.y = y;
    v->m_vPos.z = z;
    v->m_YPosOffset = yOffset;
    v->m_qGround = ground;
    v->m_pCurrentRegion = region;
    v->m_ZoomBlend = zoomBlend;
    v->m_ZoomFov = zoomFov;
    if (Cinema_PathPlaying() != 0) {
        v->m_pCurrentRegion = (CGameRegion *)CScene__FindRegionAt(&gScene, &v->m_vPos.x);
    }
    region = v->m_pCurrentRegion;
    v->m_VisBits = gDefaultVisBits;
    if (region != NULL && Cinema_PathPlaying() == 0) {
        value = ((v->m_vPos.y + v->m_YPosOffset) - CGameRegion__GetHeight(region, v->m_vPos.x, v->m_vPos.z)) * 0.09765625f;
        if (value < 11.0f && 5.0f < value) {
            v->m_VisBits = region->m_VisBits;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        v->m_VisBits = gDefaultVisBits;
    }
    if (v->m_ZoomBlend > 0.0f) {
        v->m_VisBits = gDefaultVisBits;
    }
}
#else
f32 CGameRegion__GetHeight(struct CGameRegion *, f32, f32);
void CCamera__SetViewParams(GameplayCamera *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6,
                   f32 arg7, f32 arg8, f32 arg9, Shared_Quad arg10, sptr arg11, f32 arg12,
                   f32 arg13) {
    sptr object;
    f32 value;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    arg0->unk24 = arg1;
    arg0->unk28 = arg2;
    arg0->unk2C = arg3;
    arg0->unk30 = arg4;
    arg0->unk34 = arg5;
    arg0->unk38 = arg6;
    arg0->unk3C = arg7;
    arg0->unk40 = arg8;
    arg0->unk44 = arg9;
    arg0->unk48 = arg10;
    arg0->unk58 = (void *)arg11;
    arg0->unk5C = arg12;
    arg0->unk60 = arg13;
    if (Cinema_PathPlaying() != 0) {
        arg0->unk58 = (void *)CScene__FindRegionAt(&gScene, &arg0->unk38);
    }
    object = (sptr)arg0->unk58;
    x = arg0->unk38;
    y = arg0->unk3C;
    z = arg0->unk40;
    w = arg0->unk44;
    arg0->unk64 = gDefaultVisBits;
    if (object != 0 && Cinema_PathPlaying() == 0) {
        value = ((y + w) - CGameRegion__GetHeight((struct CGameRegion *) object, x, z)) * 0.09765625f;
        if (value < 11.0f && 5.0f < value) {
            arg0->unk64 = ((MenuRules *)(object))->locked;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        arg0->unk64 = gDefaultVisBits;
    }
    if (arg0->unk5C > 0.0f) {
        arg0->unk64 = gDefaultVisBits;
    }
}
#endif
#include "structs/vector.h"

CVector3 CCamera__GetViewOrigin(u8 *arg0) {
    return *(CVector3 *)(arg0 + T2_OFFSETOF(CCamera, m_vTCorners));
}

/* Switches the camera to mode arg1 and runs its enter handler. */
void CCamera__SetMode(CCamera *arg0, s16 arg1) {
    void (*temp_v0)(void);

    arg0->m_Mode = arg1;
    arg0->m_Time = 0.0f;
    arg0->m_Stage = 0;
    arg0->m_StageTime = 0.0f;
    temp_v0 = gCameraModeTable[arg0->m_Mode].enter;
    if (temp_v0 != NULL) {
        temp_v0();
    }
}

/* Resets the under-water wobble of the view: still, at its rest scale and
 * speeds (the speeds are angles a frame: pi/2 and 75 degrees). */
void CCamera__ResetWaterWobble(CCamera *cam) {
    cam->m_WaterWobbleXAngle = 0.0f;
    cam->m_WaterWobbleYAngle = 0.0f;
    cam->m_WaterWobbleXScale = 0.2f;
    cam->m_WaterWobbleYScale = 0.1f;
    cam->m_WaterWobbleXSpeed = 1.5707965f;     /* 0x3FC90FDC: a hair over the float nearest pi/2 */
    cam->m_WaterWobbleYSpeed = 1.3089970f;
}

typedef struct {
    u8 r, g, b, a;
} Color32;

/* Fades out a red flash over the screen area and draws it while it lasts. */
void CCamera__DrawHitFlash(CCamera *scr) {
    Color32 color;

    if (scr->m_Flash > scr->m_FlashDec) {
        scr->m_Flash -= (u32)(scr->m_FlashDec * gFrameIncrement);
    } else {
        scr->m_Flash = 0;
    }
    if (scr->m_Flash != 0) {
        color.r = 0xFF;
        color.g = 0;
        color.b = 0;
        color.a = scr->m_Flash;
        CEngineApp__DrawFilledRect(&gEngineApp, (u8 *) &color, scr->m_Viewport.m_ScreenPosX, scr->m_Viewport.m_ScreenPosY,
                      scr->m_Viewport.m_ScreenPosX + scr->m_Viewport.m_Width, scr->m_Viewport.m_ScreenPosY + scr->m_Viewport.m_Height);
    }
}

/* Draws the camera's flash: a rectangle of m_FlashColor over its viewport. */
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CCamera__UpdateFlash receives CCamera__DrawFlash's own argument
 * registers (pass-through); the host passes them. */
void CCamera__DrawFlash(CCamera *cam) {
    f32 x;

    CCamera__UpdateFlash(cam);
    if (cam->m_FlashMode != 0) {
        x = cam->m_Viewport.m_ScreenPosX;
        CEngineApp__DrawFilledRect(&gEngineApp, cam->m_FlashColor, (s32) x, (s32) cam->m_Viewport.m_ScreenPosY,
                      (s32) (x + cam->m_Viewport.m_Width),
                      (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
    }
}

/* Sets the flash level and how fast it fades. */
void CCamera__SetHitFlash(CCamera *cam, s8 flash, s8 dec) {
    cam->m_Flash = flash;
    cam->m_FlashDec = dec;
}

/* Starts a screen flash: the colour, the most opaque it gets, and how many
 * frames it takes to rise, hold and fade. */
void CCamera__StartFlash(CCamera *cam, s8 maxOpacity, s8 red, s8 green, s32 blue, s32 attack, s32 sustain, s32 decay) {
    cam->m_MaxOpacity = maxOpacity;
    cam->m_FlashColor[0] = red;
    cam->m_FlashColor[1] = green;
    cam->m_FlashColor[3] = 0;
    cam->m_FlashTimer = 0.0f;
    cam->m_FlashMode = 1;
    cam->m_FlashColor[2] = blue;
    cam->m_AttackTime = attack;
    cam->m_SustainTime = sustain;
    cam->m_DecayTime = decay;
}

/* The camera's fog colour, each channel to 5 bits, or black while flag
 * 0x4000 is up. */
void CCamera__GetFogColor(CCamera *cam, u8 *red, u8 *green, u8 *blue) {
    if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
        *red = 0;
        *green = 0;
        *blue = 0;
        return;
    }
    *red = cam->m_FogColor[0] & 0xF8;
    *green = cam->m_FogColor[1] & 0xF8;
    *blue = cam->m_FogColor[2] & 0xF8;
}

/* Where a point of the world falls on the screen, through the camera's
 * projection. */
void CCamera__WorldToScreen(CCamera *cam, CVector3 *point, f32 *x, f32 *y) {
    CVector3 value;
    f32 half;

    CMtxF__TransformPointProjective(&cam->m_mfProjection, point, &value);
    half = (f32) (gScreenWidth / 2);
    *x = value.x * half + half;
    *y = value.y * (f32) (-gScreenHeight / 2) + (f32) (gScreenHeight / 2);
}

/* The player whose camera this is, or NULL. */
CPlayer *CCamera__GetPlayer(CCamera *cam) {
    CPlayer *player;

    for (player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == (void *) cam) {
            return player;
        }
    }
    return NULL;
}

/* Sets the camera's own shake source, as CCameraPool__StartShake sets one of the
 * pool's: position, radius, wave type and a tenth of each amplitude. */
void CCamera__SetShake(CCamera *cam, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, CVector3 pos) {
    u8 pad[0x18];       /* unused stack, as in CCameraPool__StartShake */
    CVector3 at;

    at = pos;
    cam->m_Shake.m_vPos = at;
    cam->m_Shake.m_Radius = radius;
    cam->m_Shake.m_XAxis.m_Type = type;
    cam->m_Shake.m_YAxis.m_Type = type;
    cam->m_Shake.m_ZAxis.m_Type = type;
    cam->m_Shake.m_XAxis.m_Shake = ampX * 0.1f;
    cam->m_Shake.m_YAxis.m_Shake = ampY * 0.1f;
    cam->m_Shake.m_ZAxis.m_Shake = ampZ * 0.1f;
}

s32 Frustum_ContainsPoint(CPlane *planes, CVector3 *point);

/* Whether a point is inside the camera's view volume (its six planes). The
 * point comes by value. */
s32 CCamera__IsPointVisible(CCamera *cam, CVector3 point) {
    return Frustum_ContainsPoint(cam->m_ViewVolume, &point);
}

/* Resets the warp: none, still, centred, no spin, scale 1. */
void CCamera__ResetWarp(CCamera *cam) {
    cam->m_WarpMode = 0;
    cam->m_WarpTime = 0.0f;
    cam->m_vWarpPos.x = 0.0f;
    cam->m_vWarpPos.y = 0.0f;
    cam->m_vWarpPos.z = 0.0f;
    cam->m_WarpSpin = 0.0f;
    cam->m_WarpXScale = 1.0f;
    cam->m_WarpYScale = 1.0f;
}

void func_002812A4(void) {

}

/* Starts warp 1. */
void CCamera__StartWarp1(CCamera *cam) {
    cam->m_WarpTime = 0.0f;
    cam->m_WarpMode = 1;
}

/* Starts warp 2. */
void CCamera__StartWarp2(CCamera *cam) {
    cam->m_WarpTime = 0.0f;
    cam->m_WarpMode = 2;
}

/* Loads the camera's on-screen (2D) projection for this frame's buffer:
 * gSPMatrix(G_MTX_PROJECTION | G_MTX_LOAD). */
void CCamera__LoadOnScreenProjection(CCamera *cam) {
    Gfx *g = gDList++;

    g->w0 = 0xDA380007;
    g->w1 = GBI_ADDR(&cam->m_mOnScreenProjection[gEvenOdd]);
}

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);     /* out = a - b */

/* How far a point is from the camera. */
f32 CCamera__DistanceTo(CCamera *cam, CVector3 *point) {
    CVector3 d;

    CVector3__Subtract(&d, &cam->m_vPos, point);
    return func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
}

void func_00281354(void) {

}

/* Starts a camera shake: a free shake source if there is one, else the
 * newest active one is reused. It shakes cameras within `radius` of `pos`,
 * each axis by its amplitude (a tenth of the value given) with the same
 * wave type. */
void CCameraPool__StartShake(CCameraPool *pool, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, CVector3 pos) {
    CShake *shake = pool->m_ShakeFreeList.m_pHead;
    u8 pad[0x18];       /* stack the original keeps here, unused (an inlined helper's, most likely) */
    CVector3 at;

    if (shake != NULL) {
        CList__Remove(&pool->m_ShakeFreeList, shake);
        CList__AddHead(&pool->m_ShakeActiveList, shake);
    } else {
        shake = pool->m_ShakeActiveList.m_pTail;
    }
    at = pos;
    shake->m_vPos = at;
    shake->m_Radius = radius;
    shake->m_XAxis.m_Type = type;
    shake->m_YAxis.m_Type = type;
    shake->m_ZAxis.m_Type = type;
    shake->m_XAxis.m_Shake = ampX * 0.1f;
    shake->m_YAxis.m_Shake = ampY * 0.1f;
    shake->m_ZAxis.m_Shake = ampZ * 0.1f;
}

void CVector3__ClampLength(f32 *v, f32 limit);

/* The active camera nearest a point, for its sound; the main camera when
 * the pool says only it is used for sound, or when none is active. */
CCamera *CCameraPool__GetNearestCamera(CCameraPool *pool, CVector3 *point) {
    CVector3 d;
    CCamera *cam;
    CCamera *best;
    f32 bestDist;
    f32 dist;

    CVector3__ClampLength(&point->x, 20000.0f);
    if (pool->m_bOnlyUseMainCameraForSound != 0) {
        return &pool->m_MainCamera;
    }
    cam = pool->m_ActiveList.m_pHead;
    bestDist = 3.4028235e38f;
    best = NULL;
    if (cam != NULL) {
        do {
            CVector3__Subtract(&d, point, &cam->m_vPos);
            dist = d.x * d.x + d.y * d.y + d.z * d.z;
            if (dist < bestDist) {
                best = cam;
                bestDist = dist;
            }
            cam = cam->m_pNext;
        } while (cam != NULL);
    }
    if (best == NULL) {
        return &pool->m_MainCamera;
    }
    return best;
}

/* How far a point is from the active cameras, on average. */
f32 CCameraPool__GetAverageDistance(CCameraPool *pool, CVector3 *point) {
    CVector3 d;
    f32 sum;
    CCamera *cam;
    f64 n;
    s32 count;

    if (pool->m_ActiveList.m_nCount == 0) {
        return 0.0f;
    }
    cam = pool->m_ActiveList.m_pHead;
    sum = 0.0f;
    if (cam != NULL) {
        do {
            CVector3__Subtract(&d, &cam->m_vPos, point);
            sum += func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
            cam = cam->m_pNext;
        } while (cam != NULL);
    }
    count = pool->m_ActiveList.m_nCount;
    n = (f64) count;
    if (count < 0) {
        n += 4294967296.0;
    }
    return sum / (f32) n;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 active;
} Unk0027FC2C;

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CCameraPool__AddText receives CCameraPool__AddSmallText's own argument
 * registers (pass-through); the host passes them. */
void *CCameraPool__AddSmallText(void * pt0, void * pt1, void * pt2) {
    void *temp_v0;

    temp_v0 = CCameraPool__AddText(pt0, pt1, pt2);
    if (temp_v0 != NULL) {
        ((Unk0027FC2C *)temp_v0)->active = 1;
    }
    return temp_v0;
}

/* Shows a string over a camera for `secs`, one text per line, centred, 80
 * pixels above the bottom, with no id (-1). Returns the last line's text. */
CCameraText *CCameraPool__AddTimedText(CCameraPool *pool, CCamera *cam, u8 *string, f32 secs) {
    CCameraText *text;
    u8 *str;
    u8 *line;
    u8 *c;
    s32 mode;
    s32 id;

    mode = 2;
    id = -1;
    str = string;
    text = NULL;
    if (str == *(u8 **) sCinemaTextFadeOut) {
        str = sCameraDefaultText;
    }
    c = str;
    line = c;
    while (*c != 0) {
        c++;
        if (*c == '\n' || *c == 0) {
            if (line != c) {
                text = TakeText(pool, cam);
                if (text != NULL) {
                    text->m_nMode = mode;
                    text->m_pString = (char *)line;
                    text->m_nAlpha = 0;
                    text->m_Scale = 1.0f;
                    text->m_DisplayTime = secs * 15.0f;
                    text->m_nId = id;
                    text->m_bSmall = 0;
                    text->m_ModeTime = 0.0f;
                    text->m_ModeDuration = 0.0f;
                    text->m_ScaleX = 1.0f;
                    text->m_ScaleY = 1.0f;
                    text->m_PositionX = gScreenWidth / 2;
                    text->m_PositionY = gScreenHeight - 80;
                }
                line = c + 1;
            }
        }
    }
    return text;
}

/* CCameraPool__AddMultiLineText (LibTEngine's name): shows a string over a
 * camera for `secs`, one text per line, centred, 80 pixels above the bottom.
 * Texts the camera shows under the same id go first. Returns the last line's
 * text. */
CCameraText *CCameraPool__AddMultiLineText(CCameraPool *pool, CCamera *cam, u8 *string, sptr id, f32 secs) {
    CCameraText *cur;
    CCameraText *next;
    CCameraText *text;
    u8 *str;
    u8 *line;
    u8 *c;
    s32 mode;

    cur = cam->m_ActiveTextList.m_pHead;
    while (cur != NULL) {
        next = cur->m_pNext;
        if (cur->m_nId == id) {
            CList__Remove(&cam->m_ActiveTextList, cur);
            CList__AddHead(&pool->m_FreeTextList, cur);
        }
        cur = next;
    }
    mode = 2;
    str = string;
    text = NULL;
    if (str == *(u8 **) sCinemaTextFadeOut) {
        str = sCameraDefaultText;
    }
    c = str;
    line = c;
    while (*c != 0) {
        c++;
        if (*c == '\n' || *c == 0) {
            if (line != c) {
                text = TakeText(pool, cam);
                if (text != NULL) {
                    text->m_nMode = mode;
                    text->m_pString = (char *)line;
                    text->m_nAlpha = 0;
                    text->m_Scale = 1.0f;
                    text->m_DisplayTime = secs * 15.0f;
                    text->m_nId = id;
                    text->m_bSmall = 0;
                    text->m_ModeTime = 0.0f;
                    text->m_ModeDuration = 0.0f;
                    text->m_ScaleX = 1.0f;
                    text->m_ScaleY = 1.0f;
                    text->m_PositionX = gScreenWidth / 2;
                    text->m_PositionY = gScreenHeight - 80;
                }
                line = c + 1;
            }
        }
    }
    return text;
}

/* The block at arg0 + 0x40 of CCameraPool__ReleaseAllTexts: a list at 0xE28 whose elements go back to the list at arg0 + 0xF0C. */
typedef struct {
    /* 0x000 */ u8 pad0[0xE28];
    /* 0xE28 */ CList m_List;
} ListOwner;

void CCameraPool__ReleaseCameraTexts(s32 arg0, ListOwner *arg1) {
    s32 *temp_s0;
    s32 *var_s1;
    void *var_a0;

    var_s1 = arg1->m_List.m_pTail;
    if (var_s1 != NULL) {
        var_a0 = &arg1->m_List;
        do {
            temp_s0 = *var_s1;
            CList__Remove(var_a0, var_s1);
            CList__AddHead(arg0 + 0xF0C, var_s1);
            var_s1 = temp_s0;
            var_a0 = &arg1->m_List;
        } while (var_s1 != NULL);
    }
}

/* The main camera (pool + 0x40) and each active camera give their active texts (CCamera
 * m_ActiveTextList, 0xE28) back to the pool's free texts (m_FreeTextList, 0xF0C). */
#define TEXTS T2_OFFSETOF(CCamera, m_ActiveTextList)
#define FREE_TEXTS T2_OFFSETOF(CCameraPool, m_FreeTextList)
#define MAIN_CAMERA T2_OFFSETOF(CCameraPool, m_MainCamera)

void CCameraPool__ReleaseAllTexts(sptr arg0)
{
  s32 *temp_s0;
  s32 *temp_s0_2;
  s32 *var_s1;
  s32 *var_s1_2;
  void *temp_s2;
  void *var_a0;
  void *var_a0_2;
  void *var_s2;
  temp_s2 = arg0 + MAIN_CAMERA;
  var_s1 = *((s32 **) (((s8 *) temp_s2) + TEXTS + T2_OFFSETOF(CList, m_pTail)));
  var_a0 = temp_s2 + TEXTS;
  if (var_s1 != 0)
  {
    do
    {
      temp_s0 = *(s32 **) var_s1; /* CCameraText.m_pLast, pointer-wide */
      CList__Remove((sptr) (temp_s2 + TEXTS), var_s1);
      CList__AddHead(arg0 + FREE_TEXTS, var_s1);
      var_s1 = temp_s0;
      var_a0 = temp_s2 + TEXTS;
    }
    while (var_s1 != 0);
  }
  var_s2 = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
  if (var_s2 != 0)
  {
    do
    {
      var_s1_2 = *((s32 **) (((s8 *) var_s2) + TEXTS + T2_OFFSETOF(CList, m_pTail)));
      var_a0_2 = var_s2 + TEXTS;
      if (var_s1_2 != 0)
      {
        do
        {
          temp_s0_2 = *(s32 **) var_s1_2;
          CList__Remove(var_a0_2, var_s1_2);
          CList__AddHead(arg0 + FREE_TEXTS, var_s1_2);
          var_s1_2 = temp_s0_2;
          var_a0_2 = var_s2 + TEXTS;
        }
        while (var_s1_2 != 0);
      }
      var_s2 = *((void **) (((s8 *) var_s2) + T2_OFFSETOF(CCamera, m_pNext)));
    }
    while (var_s2 != 0);
  }
}

void CCamera__ConstructSkySystem(CCamera *cam) {
    CEmitterPool__Construct((struct CEmitterPool *) cam->m_SkySystem);
}

/* Takes a free text from the pool and puts it on the camera's active texts. */
CCameraText *CCameraPool__TakeText(CCameraPool *pool, CCamera *cam) {
    CCameraText *text = pool->m_FreeTextList.m_pHead;

    if (text != NULL) {
        CCameraText__Reset(text);
        CList__Remove(&pool->m_FreeTextList, text);
        CList__AddTail(&cam->m_ActiveTextList, text);
    }
    return text;
}

/* A camera's fading text goes back to the pool's free list. */
void CCameraPool__ReleaseText(CCameraPool *pool, CCamera *camera, struct CCameraText *text) {
    CList__Remove(&camera->m_ActiveTextList, text);
    CList__AddHead(&pool->m_FreeTextList, text);
}

/* Resets a camera text (see CCameraPool__AddTextEx.c): two words cleared. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 unk1C;
} TextReset;

/* The host body on the members (round 12 a5, tools/port/n64viewcheck.py): m_nId is pointer-wide
 * on the host (round 11 z3), so the view's 0x1C cleared the padding after m_nAlpha and left the
 * owner id of the previous text. */
void CCameraText__Reset(CCameraText *arg0) {
    arg0->m_nMode = 0;
    arg0->m_nId = 0;
}


void CShakeAxis__Construct(CShakeAxis *arg0) {
    arg0->m_Shake = 0.0f;
    arg0->m_LastShake = 0.0f;
    arg0->m_Amp = 0.0f;
    arg0->m_Angle = 0.0f;
}

void CShakeAxis__Set(CShakeAxis *arg0, f32 arg1, s32 arg2) {
    arg0->m_Type = arg2;
    arg0->m_Shake = (f32) (arg1 * 0.1f);
}
f32 CShakeAxis__GetOffset(struct Wave *wave);

f32 CShakeAxis__GetOffset(struct Wave *wave) {
    switch (wave->kind) {
    case 1:
        return func_002982F0(wave->phase * 0.017453294f) * wave->amplitude;
    case 0:
        return Random_Blend(-wave->amplitude, wave->amplitude);
    }
    return 0.0f;
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 a;
    /* 0x08 */ s32 b;
    /* 0x0C */ s32 c;
    /* 0x10 */ s32 d;
} Block00281C08; /* size 0x14 */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 scale;
    /* 0x18 */ Block00281C08 blocks[3];
} Unk00281C08;
#ifdef TARGET_PORT
#include "structs/camera.h"
/* Round 16 e1: Unk00281C08 is CShake at the N64 offsets (two 4-byte list pointers, then m_vPos at
 * 0x08, m_Radius at 0x14, the three CShakeAxis from 0x18). On the host the pointers are 8 bytes, so the
 * view zeroed the high half of m_pNext and m_vPos.x, put the radius 1.0 into m_vPos.y and left
 * m_Radius 0: CShake__ApplyOffset (distance 0 < radius 1 on the N64) then never drew its three
 * Random_Blend per camera per frame, so the port's RNG took 6 numbers per game frame in the lair where
 * the N64 takes 12 (n64trace.py --calls Random_Blend; PLAYTHROUGH.md 27). The same writes at the
 * host offsets of the members. */
void CShake__Construct(Unk00281C08 *arg0) {
    CShake *shake = (CShake *) arg0;
    CShakeAxis *axis[3];
    s32 i;

    axis[0] = &shake->m_XAxis;
    axis[1] = &shake->m_YAxis;
    axis[2] = &shake->m_ZAxis;
    *(s32 *) &shake->m_vPos.x = 0;
    *(s32 *) &shake->m_vPos.y = 0;
    *(s32 *) &shake->m_vPos.z = 0;
    shake->m_Radius = 1.0f;
    for (i = 0; i < 3; i++) {
        *(s32 *) &axis[i]->m_Shake = 0;
        *(s32 *) &axis[i]->m_LastShake = 0;
        *(s32 *) &axis[i]->m_Amp = 0;
        *(s32 *) &axis[i]->m_Angle = 0;
    }
}
#else
void CShake__Construct(Unk00281C08 *arg0) {
    Block00281C08 *temp_a0;
    Block00281C08 *temp_v0;
    Block00281C08 *temp_v0_2;

    temp_v0 = &arg0->blocks[0];
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk10 = 0;
    arg0->scale = 1.0f;
    temp_v0->a = 0;
    temp_v0->b = 0;
    temp_v0->c = 0;
    temp_v0->d = 0;
    temp_v0_2 = &arg0->blocks[1];
    temp_a0 = &arg0->blocks[2];
    temp_v0_2->a = 0;
    temp_v0_2->b = 0;
    temp_v0_2->c = 0;
    temp_v0_2->d = 0;
    temp_a0->a = 0;
    temp_a0->b = 0;
    temp_a0->c = 0;
    temp_a0->d = 0;
}
#endif

void CShake__Set(struct RecordRecAt18 *record, f32 first, f32 second, f32 third, f32 value, s32 mode, Triple triple);

void CShake__Set(struct RecordRecAt18 *record, f32 first, f32 second, f32 third, f32 value, s32 mode,
                   Triple triple) {
    record->triple = triple;
    record->value = value;
    record->channels[0].mode = mode;
    record->channels[1].mode = mode;
    record->channels[2].mode = mode;
    record->channels[0].level = first * 0.1f;
    record->channels[1].level = second * 0.1f;
    record->channels[2].level = third * 0.1f;
}

s32 CShake__Advance(s32 arg0) {
    s32 var_v1;
    u32 temp_s0;
    u32 temp_s1;
    u32 temp_v0;

    temp_s1 = CShakeAxis__Advance(arg0 + 0x18);
    temp_s0 = CShakeAxis__Advance(arg0 + 0x2C);
    temp_v0 = CShakeAxis__Advance(arg0 + 0x40);
    var_v1 = 0;
    if ((temp_s1 != 0) && (temp_s0 != 0)) {
        var_v1 = temp_v0 > 0U;
    }
    return var_v1;
}

/* What this reads of a region: its flags. 0x10000 makes the ground tremble. */
typedef struct {
    /* 0x00 */ u8 _pad[0x44];
    /* 0x44 */ s32 m_Flags;
} RegionTremor;
f32 Random_Blend(f32 min, f32 max);          /* a random float between the two */

/* The tremor of the ground in a region that shakes: eases the camera's
 * tremor toward 0.5 degrees and 0.25 high there (toward nothing elsewhere)
 * and gives this frame's random offset of the view. */
void CCamera__UpdateTremor(CCamera *cam, CVector3 *offset) {
    f32 height = 0.0f;
    f32 angle = height;
    RegionTremor *region = CScene__GetRegionInfo(&gScene, cam->m_ViewParams.m_pCurrentRegion);

    if (region == NULL) {
        offset->x = height;
        offset->y = height;
        offset->z = height;
        return;
    }
    if (region->m_Flags & 0x10000) {
        angle = 0.5f;
        height = 0.25f;
    }
    cam->m_TremorAngle += (angle - cam->m_TremorAngle) * 0.2f;
    cam->m_TremorHeight += (height - cam->m_TremorHeight) * 0.1f;
    offset->x = Random_Blend(-cam->m_TremorAngle * 0.017453294f, cam->m_TremorAngle * 0.017453294f);
    offset->y = Random_Blend(0.0f, cam->m_TremorHeight * 10.24f);
    offset->z = Random_Blend(-cam->m_TremorAngle * 0.017453294f, cam->m_TremorAngle * 0.017453294f);
}

void func_00281E44(void) {

}

s32 Cinema_PathPlaying();                                /* extern */

/* Picks the visibility bits of the camera: those of its region when it is 5 to 11 units
 * above the floor there, else the default gDefaultVisBits. */
void CCamera__UpdateVisBits(CCamera *arg0) {
    f32 temp_f1;
    f32 temp_f20;
    f32 temp_f21;
    f32 temp_f22;
    f32 temp_f23;
    struct CGameRegion *temp_s1;

    if (Cinema_PathPlaying() != 0) {
        arg0->m_ViewParams.m_pCurrentRegion = CScene__FindRegionAt(&gScene, &arg0->m_ViewParams.m_vPos);
    }
    temp_s1 = arg0->m_ViewParams.m_pCurrentRegion;
    temp_f21 = arg0->m_ViewParams.m_vPos.x;
    temp_f23 = arg0->m_ViewParams.m_vPos.y;
    temp_f20 = arg0->m_ViewParams.m_vPos.z;
    temp_f22 = arg0->m_ViewParams.m_YPosOffset;
    arg0->m_ViewParams.m_VisBits = gDefaultVisBits;
    if ((temp_s1 != 0) && (Cinema_PathPlaying() == 0)) {
        temp_f1 = ((temp_f23 + temp_f22) - CGameRegion__GetHeight(temp_s1, temp_f21, temp_f20)) * 0.09765625f;
        if ((temp_f1 < 11.0f) && (temp_f1 > 5.0f)) {
            arg0->m_ViewParams.m_VisBits = temp_s1->m_VisBits;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        arg0->m_ViewParams.m_VisBits = gDefaultVisBits;
    }
    if (arg0->m_ViewParams.m_ZoomBlend > 0.0f) {
        arg0->m_ViewParams.m_VisBits = gDefaultVisBits;
    }
}

/* Allocates the pool's arg1 cameras and clears them. */
void CCameraPool__AllocCameras(CCameraPool *arg0, s32 arg1) {
    s32 *temp_v0;
    sptr temp_a0;
    s32 temp_s0;

    temp_s0 = arg1 * sizeof(CCamera);
    arg0->m_nCameras = arg1;
    temp_v0 = (s32 *) CCache__Allocate(0, temp_s0, 0x23, sCCameraPoolCacheDesc_Cameras);
    arg0->m_pmeCameras = (struct CMemEntry *) temp_v0;
    temp_a0 = *temp_v0;
    arg0->m_Cameras = (struct CCamera *) temp_a0;
    memset((void *) temp_a0, 0, temp_s0);
}

/* Allocates the pool's on-screen texts (0x40 bytes each) and clears them. */
void CCameraPool__AllocTexts(CCameraPool *pool, s32 count) {
    s32 size = count * sizeof(CCameraText);

    pool->m_nTexts = count;
    pool->m_pmeTexts = CCache__Allocate(0, size, 0x23, sCCameraPoolCacheDesc_CameraTexts);
    pool->m_Texts = *(CCameraText **) pool->m_pmeTexts;
    memset(pool->m_Texts, 0, size);
}

/* Frees the pool's cameras, the main one first. */
void CCameraPool__FreeCameras(CCameraPool *pool) {
    pool->m_bOnlyUseMainCameraForSound = 1;
    Sound__SetListener(&pool->m_MainCamera);
    CCache__Flush(0);
    if (pool->m_pmeCameras != NULL) {
        CCache__Deallocate(0, pool->m_pmeCameras);
        pool->m_pmeCameras = NULL;
        pool->m_Cameras = NULL;
        pool->m_nCameras = 0;
    }
}

/* Frees the pool's on-screen texts. */
void CCameraPool__FreeTexts(CCameraPool *pool) {
    CCache__Flush(0);
    if (pool->m_pmeTexts != NULL) {
        CCache__Deallocate(0, pool->m_pmeTexts);
        pool->m_pmeTexts = NULL;
        pool->m_Texts = NULL;
        pool->m_nTexts = 0;
    }
}

/* Moves value toward target by step, without overshooting; jumps straight
 * there when SavedPlayers_IsActive says so. While it moves, gScreenClearTime is set
 * to 30. */
f32 CCameraPool__StepToward(CCameraPool *pool, f32 value, f32 target, f32 step) {
    if (SavedPlayers_IsActive() != 0) {
        value = target;
    }
    if (value < target) {
        value += step;
        gScreenClearTime = 30.0f;
        if (value > target) {
            value = target;
        }
    } else if (value > target) {
        value -= step;
        gScreenClearTime = 30.0f;
        if (value < target) {
            value = target;
        }
    }
    return value;
}

/* Shows a string over a camera for `secs`, one text per line, centred, 80
 * pixels above the bottom, starting in the given mode and with the given id.
 * Returns the last line's text. */
CCameraText *CCameraPool__AddTextEx(CCameraPool *pool, CCamera *cam, u8 *string, s32 mode, f32 secs, s32 id) {
    CCameraText *text;
    u8 *line;
    u8 *c;

    text = NULL;
    if (string == *(u8 **) sCinemaTextFadeOut) {
        string = sCameraDefaultText;
    }
    c = string;
    line = c;
    while (*c != 0) {
        c++;
        if (*c == '\n' || *c == 0) {
            if (line != c) {
                text = TakeText(pool, cam);
                if (text != NULL) {
                    text->m_nMode = mode;
                    text->m_pString = (char *)line;
                    text->m_nAlpha = 0;
                    text->m_Scale = 1.0f;
                    text->m_DisplayTime = secs * 15.0f;
                    text->m_nId = id;
                    text->m_bSmall = 0;
                    text->m_ModeTime = 0.0f;
                    text->m_ModeDuration = 0.0f;
                    text->m_ScaleX = 1.0f;
                    text->m_ScaleY = 1.0f;
                    text->m_PositionX = gScreenWidth / 2;
                    text->m_PositionY = gScreenHeight - 80;
                }
                line = c + 1;
            }
        }
    }
    return text;
}

#include "structs/list.h"

VM_TEXT void CCameraPool__Construct(CCameraPool *pool) {
    CCamera *main;
    memset(pool, 0, sizeof(CCameraPool));
    CList__Construct(&pool->m_FreeList, 0, sizeof(void *));
    CList__Construct(&pool->m_ActiveList, 0, sizeof(void *));
    CList__Construct(&pool->m_FreeTextList, 0, sizeof(void *));
    main = &pool->m_MainCamera;
    pool->m_bOnlyUseMainCameraForSound = 1;
    pool->m_pmeCameras = 0;
    pool->m_Cameras = 0;
    pool->m_nCameras = 0;
    pool->m_pmeTexts = 0;
    pool->m_Texts = 0;
    pool->m_nTexts = 0;
    pool->m_bActiveChanged = 2;
    CCamera__Construct(main);
    CCamera__Advance(main);
}

#ifdef TARGET_PORT
#include "structs/camera.h"

void CEmitterPool__Construct(struct CEmitterPool *);

/* Host body: the pool and its cameras through CCameraPool/CCamera (the view below has the camera
 * table and count at their N64 offsets 4/8 and the 0xEC0 camera stride). */
VM_TEXT void CCameraPool__ResetEmitterPools(CCameraPool *owner) {
    s32 i;

    for (i = 0; i < owner->m_nCameras; i++) {
        CEmitterPool__Construct((struct CEmitterPool *)owner->m_Cameras[i].m_SkySystem);
    }
    CEmitterPool__Construct((struct CEmitterPool *)owner->m_MainCamera.m_SkySystem);
}
#else
typedef struct Cameras {
    /* 0x0 */ s32 handle;
    /* 0x4 */ s32 cameras; /* address of count 0xEC0-byte records */
    /* 0x8 */ s32 count;
} Cameras;

void CEmitterPool__Construct(struct CEmitterPool *);

/* Updates each camera's message block (at 0x558), then the owner's own at 0x598. */
VM_TEXT void CCameraPool__ResetEmitterPools(Cameras *owner) {
    sptr i;

    for (i = 0; i < owner->count; i++) {
        CEmitterPool__Construct((struct CEmitterPool *)(i * 0xEC0 + owner->cameras + 0x558));
    }
    CEmitterPool__Construct((struct CEmitterPool *)((char *)owner + 0x598));
}
#endif

/* Takes the first free camera, resets it and moves it to the active list. */
VM_TEXT CCamera *CCameraPool__AllocCamera(CCameraPool *arg0) {
    CCamera *temp_s0;

    temp_s0 = (CCamera *) arg0->m_FreeList.m_pHead;
    if (temp_s0 != NULL) {
        CCamera__Construct(temp_s0);
        CList__Remove(&arg0->m_FreeList, temp_s0);
        CList__AddTail(&arg0->m_ActiveList, temp_s0);
    }
    return temp_s0;
}

VM_TEXT void CCameraPool__FreeCamera(sptr arg0, sptr arg1) {
    CList__Remove(arg0 + 0x20, arg1);
    CList__AddHead(arg0 + 0xC, arg1);
}

VM_TEXT void CEmitterPool__Construct(CEmitterPool *arg0) {
    CEmitter *var_s0;
    s32 var_s1;

    CList__Construct(&arg0->m_ActiveList, T2_OFFSETOF(CEmitter, m_pLast), T2_OFFSETOF(CEmitter, m_pNext));
    CList__Construct(&arg0->m_FreeList, T2_OFFSETOF(CEmitter, m_pLast), T2_OFFSETOF(CEmitter, m_pNext));
    var_s1 = 0;
    var_s0 = arg0->m_Emitters;
    do {
        CList__AddTail(&arg0->m_FreeList, var_s0);
        var_s1 += 1;
        var_s0++;
    } while (var_s1 < 4);
    arg0->m_ActiveMask = 0;
}
