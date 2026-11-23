/* FAKEMATCH (round 22): 2 empty `__asm__` barriers around the N64 write of the gDPSetTextureFilter words in
 * CCamera__DrawGlares (the round-21 scan missed them: they sit in the #else of a TARGET_PORT pair); the
 * port's form (one Gfx through gDList++) on the N64: 13 of 98 words. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars),
 * src/game/func_00238D20.c, with Turok 2's CCamera. */
#include "common.h"
#include "structs/camera.h"
#include "gfx.h"
#include "globals/render.h"
#include "structs/particle.h"

/* The glares are particles whose template flags have 0x80000; m_dwFlags is the camera mask. */
extern CParticle *D_8010753C; /* the particle draw list head (gendata aliases it to the host member) */
void Hud_SetupTextureMode(s32 a);
void Render_SetRenderMode(s32 a);
void CMtxF__TransformPointProjective(CMtxF *m, CVector3 *in, CVector3 *out);
void CCamera__DrawGlare(CCamera *cam, f32 x, f32 y, f32 scale, s32 c, s32 d);

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
    for (e = D_8010753C; e != NULL; e = e->m_pDrawNext) {
        if ((e->m_dwFlags & mask) && (e->m_pEffect->m_dwFlags & 0x80000)) {
            ScreenPos(cam, &e->header.m_vPos, &sx, &sy);
            CCamera__DrawGlare(cam, sx, sy, e->m_Alpha * 0.003921569f, 0, 0);
        }
    }
}
