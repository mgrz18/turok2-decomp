/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars),
 * src/game/func_00238D20.c, with Turok 2's CCamera. */
#include "common.h"
#include "structs/camera.h"
#include "gfx.h"
#include "globals/render.h"
#ifdef TARGET_PORT
#include "structs/particle.h"
#endif

/* An object that can shine a glare (only the fields read here). */
#ifdef TARGET_PORT
typedef struct CGlareSource {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_CGlareSource(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_m_vPos[T2_HOFF_CGlareSource(header.m_vPos)]; CVector3 m_vPos; }; /* N64 0x4 */
        struct { u8 _hp_m_CameraMask[T2_HOFF_CGlareSource(m_dwFlags)]; u32 m_CameraMask; }; /* N64 0x40 */
        struct { u8 _hp_m_pFlags[T2_HOFF_CGlareSource(m_pEffect)]; u32 *m_pFlags; }; /* N64 0xF8 */
        struct { u8 _hp_m_GlareScale[T2_HOFF_CGlareSource(m_Alpha)]; f32 m_GlareScale; }; /* N64 0x174 */
        struct { u8 _hp_m_pNext[T2_HOFF_CGlareSource(m_pDrawNext)]; struct CGlareSource *m_pNext; }; /* N64 0x1D0 */
    };
} CGlareSource;
#else
typedef struct CGlareSource {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CVector3 m_vPos;
    /* 0x010 */ u8 pad10[0x30];
    /* 0x040 */ u32 m_CameraMask;           /* bit 3 + camera index: seen by it */
    /* 0x044 */ u8 pad44[0xB4];
    /* 0x0F8 */ u32 *m_pFlags;              /* 0x80000: shines */
    /* 0x0FC */ u8 padFC[0x78];
    /* 0x174 */ f32 m_GlareScale;
    /* 0x178 */ u8 pad178[0x58];
    /* 0x1D0 */ struct CGlareSource *m_pNext;
} CGlareSource;
#endif

extern CGlareSource *D_8010753C; /* the particle draw list head (gendata aliases it to the host member) */
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
    CGlareSource *e;
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
    for (e = D_8010753C; e != NULL; e = e->m_pNext) {
        if ((e->m_CameraMask & mask) && (*e->m_pFlags & 0x80000)) {
            ScreenPos(cam, &e->m_vPos, &sx, &sy);
            CCamera__DrawGlare(cam, sx, sy, e->m_GlareScale * 0.003921569f, 0, 0);
        }
    }
}
