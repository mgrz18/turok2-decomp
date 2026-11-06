/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A170_de, with Turok 2's layouts. */
/* Second VM module copy of CCamera__Construct (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/camera.h"

typedef struct Floats {
    f32 unk0, unk4, unk8, unkC, unk10, unk14;
} Floats;

typedef struct IntPair {
    s32 field_0;
    s32 field_4;
} IntPair;

extern s32 gScreenWidth;
extern s32 gScreenHeight;
extern IntPair gCameraVisBits;

void CCameraViewParams__Construct(void *);
void CMtxF__Identity(void *);
void CCamera__ResetWarp(CCamera *);
void CCamera__SetMode(CCamera *, s32);
void MenuPool_Construct_2(void *);
void CEmitterPool__Construct_2(void *);
void CList__Construct(void *list, s32 prevOffset, s32 nextOffset);
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
    CList__Construct(&view->m_ActiveTextList, 0, 4);
    CCamera__ConstructSkySystem(view);
}
