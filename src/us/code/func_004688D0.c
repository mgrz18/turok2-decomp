/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A170_de, with Turok 2's layouts. */
/* Second VM module copy of func_0042F9B0 (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/camera.h"

typedef struct Floats {
    f32 unk0, unk4, unk8, unkC, unk10, unk14;
} Floats;

typedef struct IntPair {
    s32 field_0;
    s32 field_4;
} IntPair;

extern s32 D_800C2030;
extern s32 D_800C2034;
extern IntPair D_800F5A40;

void func_00280C28(void *);
void func_0020F79C(void *);
void func_00281278(CCamera *);
void func_00280E64(CCamera *, s32);
void func_00416614(void *);
void func_0042FF10(void *);
void CList__Construct(void *list, s32 prevOffset, s32 nextOffset);
void func_00281A98(CCamera *);

void func_004688D0(CCamera *view) {
    f32 one;
    f32 zero;
    s16 size;
    s32 i;
    Floats *f1;
    Floats *f2;
    Floats *f3;
    Floats *f4;
    Floats *f5;

    func_00280C28(&view->m_ViewParams);
    func_0020F79C(view->m_mfViewOrient);
    func_0020F79C(view->m_mfProjection);
    zero = 0.0f;
    one = 1.0f;
    f1 = (Floats *)&view->m_qViewAlign;
    f1->unk8 = zero;
    f1->unk4 = zero;
    view->m_qViewAlign.x = zero;
    view->m_qViewAlign.w = one;
    func_0020F79C(view->m_mfInverseViewOrient);
    view->m_Viewport.m_Width = (f32)D_800C2030;
    view->m_Viewport.m_Height = (f32)D_800C2034;
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
    func_00281278(view);
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
    func_00280E64(view, 0);
    size = D_800C2030 * 2;
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
    func_00416614(view->m_MenuPool);
    func_0042FF10(view->m_SkySystem);
    func_0020F79C(view->m_mfFaceScreen);
    func_0020F79C(view->m_mfFaceScreenYAxisUp);
    view->m_FlashMode = 0;
    D_800F5A40.field_4 = 0;
    CList__Construct(&view->m_ActiveTextList, 0, 4);
    func_00281A98(view);
}
