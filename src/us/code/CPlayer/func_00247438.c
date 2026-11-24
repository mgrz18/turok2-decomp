#include "common.h"
#include "structs/camera.h"
#include "gfx.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "structs/player.h"

typedef struct { s32 m[4][4]; } Mtx64;   /* a packed N64 Mtx, 0x40 bytes */

/* A model shown in the HUD (the weapon held up to the view). */


extern f32 D_800ADDB8;

void Render_InvalidateModeCache(void);
void Render_ResetBatches(void);
void Render_FlushBatches(void);
void CEngineApp__ClearDepthRect(void *vp, s32 left, s32 right, s32 top, s32 bottom);
void CMtxF__SetScale(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__RotateY(f32 *mtx, f32 angle);
void CMtxF__RotateX(void *, f32);
void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__SwapYZRows(f32 *mtx);
void CMtxF__Copy(f32 *dst, f32 *src);
void CMtxF__ScaleColumns(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__ToMtxAffine(f32 *src, Mtx64 *dst);
void CMtxF__TransformPoint(CMtxF, CVector3 *, CVector3 *);
void Render_QueueModelByKey(sptr key, void *matrix, void *lights, s32 lod, s32 pass);

void CPlayer__DrawScope(CPlayerOverlay *m, CCamera *view) {
    f32 mf[4][4];
    f32 mfDraw[4][4];
    CVector3 out;

    Render_InvalidateModeCache();
    if (m->m_rpObject == 0 || m->m_Mode == 0) {
        return;
    }
    gPrimAlphaOverride = 1;
    gPrimAlphaOverrideValue = 0x20;
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3001801;
        g->w1 = 0xC0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000C00;
        g->w1 = 0x80000;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3001201;
        g->w1 = 0x2000;
    }
    Render_ResetBatches();
    CEngineApp__ClearDepthRect(&gEngineApp, view->m_Viewport.m_ScreenPosX, view->m_Viewport.m_Width + view->m_Viewport.m_ScreenPosX, view->m_Viewport.m_ScreenPosY, view->m_Viewport.m_Height + view->m_Viewport.m_ScreenPosY);
    CMtxF__SetScale(&mf[0][0], m->m_vScale.x, m->m_vScale.y, m->m_vScale.z);
    CMtxF__RotateY(&mf[0][0], m->m_vRot.y);
    CMtxF__RotateX(&mf[0][0], m->m_vRot.x);
    CMtxF__Translate(&mf[0][0], m->m_vPos.x, m->m_vPos.y, m->m_vPos.z);
    CMtxF__SwapYZRows(&mf[0][0]);
    CMtxF__Copy(&mfDraw[0][0], &mf[0][0]);
    if (gResolution == 2) {
        CMtxF__ScaleColumns(&mfDraw[0][0], D_800ADDB8, 1.0f, 1.0f);
    }
    CMtxF__ToMtxAffine(&mfDraw[0][0], &m->m_mOrient[gEvenOdd]);
    CMtxF__TransformPoint(&mf[0][0], &m->m_vPos, &out);
    Render_QueueModelByKey(m->m_rpObject, &m->m_mOrient[gEvenOdd], &gSceneLights, 0, -1);
    Render_FlushBatches();
    gPrimAlphaOverride = 0;
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3001801;
        g->w1 = 0;
    }
}
