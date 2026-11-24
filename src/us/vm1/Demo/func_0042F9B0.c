/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A170_de, with Turok 2's layouts. */
#include "common.h"
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
void CCamera__ResetWarp(CCamera *);
void CCamera__SetMode(CCamera *, s32);
void MenuPool_Construct(void *);
void CEmitterPool__Construct(struct CEmitterPool *);
void CCamera__ConstructSkySystem(CCamera *);

void CCamera__Construct(CCamera *view) {
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
