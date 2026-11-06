#include "common.h"
#include "gfx.h"
#include "structs/vector.h"
#include "structs/game.h"

typedef struct { s32 m[4][4]; } Mtx64;   /* a packed N64 Mtx, 0x40 bytes */

/* A model shown in the HUD (the weapon held up to the view). */
typedef struct HudModel {
    /* 0x00 */ s32 current;
    /* 0x04 */ s32 requested;
    /* 0x08 */ s32 object;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ Mtx64 mtx[2];        /* one per frame buffer */
    /* 0x90 */ f32 scaleX;
    /* 0x94 */ f32 scaleY;
    /* 0x98 */ f32 scaleZ;
    /* 0x9C */ f32 rotX;
    /* 0xA0 */ f32 rotY;
    /* 0xA4 */ u8 padA4[4];
    /* 0xA8 */ CVector3 pos;
    /* 0xB4 */ s32 state;
    /* 0xB8 */ f32 time;
} HudModel;

typedef struct {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ f32 width;
    /* 0x28C */ f32 height;
    /* 0x290 */ f32 left;
    /* 0x294 */ f32 top;
} HudView;

extern u8 gPrimAlphaOverride;
extern u8 gPrimAlphaOverrideValue;
extern u8 gSceneLights[];     /* lights */
extern s32 gEvenOdd;      /* frame buffer index */
extern s32 gResolution;
extern f32 D_800ADDB8;

void Render_InvalidateModeCache(void);
void Render_ResetBatches(void);
void Render_FlushBatches(void);
void CEngineApp__ClearDepthRect(void *vp, s32 left, s32 right, s32 top, s32 bottom);
void CMtxF__SetScale(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__RotateY(f32 *mtx, f32 angle);
void CMtxF__RotateX(f32 *mtx, f32 angle);
void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__SwapYZRows(f32 *mtx);
void CMtxF__Copy(f32 *dst, f32 *src);
void CMtxF__ScaleColumns(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__ToMtxAffine(f32 *src, Mtx64 *dst);
void CMtxF__TransformPoint(f32 *mtx, CVector3 *in, CVector3 *out);
void Render_QueueModelByKey(s32 key, void *matrix, void *lights, s32 lod, s32 pass);

void CPlayer__DrawScope(HudModel *m, HudView *view) {
    f32 mf[4][4];
    f32 mfDraw[4][4];
    CVector3 out;

    Render_InvalidateModeCache();
    if (m->object == 0 || m->state == 0) {
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
    CEngineApp__ClearDepthRect(&gEngineApp, view->left, view->width + view->left, view->top, view->height + view->top);
    CMtxF__SetScale(&mf[0][0], m->scaleX, m->scaleY, m->scaleZ);
    CMtxF__RotateY(&mf[0][0], m->rotY);
    CMtxF__RotateX(&mf[0][0], m->rotX);
    CMtxF__Translate(&mf[0][0], m->pos.x, m->pos.y, m->pos.z);
    CMtxF__SwapYZRows(&mf[0][0]);
    CMtxF__Copy(&mfDraw[0][0], &mf[0][0]);
    if (gResolution == 2) {
        CMtxF__ScaleColumns(&mfDraw[0][0], D_800ADDB8, 1.0f, 1.0f);
    }
    CMtxF__ToMtxAffine(&mfDraw[0][0], &m->mtx[gEvenOdd]);
    CMtxF__TransformPoint(&mf[0][0], &m->pos, &out);
    Render_QueueModelByKey(m->object, &m->mtx[gEvenOdd], gSceneLights, 0, -1);
    Render_FlushBatches();
    gPrimAlphaOverride = 0;
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3001801;
        g->w1 = 0;
    }
}
