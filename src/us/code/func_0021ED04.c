#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/gbi.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "globals/engine.h"

typedef struct Slot24 {
    s32 w[6];
} Slot24;

#ifdef TARGET_PORT
#include "structs/camera.h"
#include "structs/model.h"
/* Model is the view's CCamera (m_mfProjection, m_mProjection[2]) and ModelRef the
 * CDrawArgs: their host members (the N64 offsets put the shadow's projection, and the
 * one restored after it, on the wrong bytes: every actor drawn after a shadow got it) */
typedef struct Model {
    union {
        struct { u8 _hp_unk1CC[__builtin_offsetof(CCamera, m_mfProjection)]; u8 unk1CC[0x40]; };
        struct { u8 _hp_mtx[__builtin_offsetof(CCamera, m_mProjection)]; Mtx mtx[2]; };
    };
} Model;

typedef struct ModelRef {
    union {
        struct { u8 _hp_model[__builtin_offsetof(CDrawArgs, m_pCamera)]; Model *model; };
        struct { u8 _hp_unkC[__builtin_offsetof(CDrawArgs, m_hModel)]; sptr unkC; };
        struct { u8 _hp_unk1C[__builtin_offsetof(CDrawArgs, m_nShadowModel)]; s32 unk1C; };
    };
} ModelRef;
#else
typedef struct Model {
    u8 pad0[0x1CC];
    u8 unk1CC[0x368 - 0x1CC];   /* 0x1CC */
    Mtx mtx[2];                 /* 0x368: one per frame buffer */
} Model;

typedef struct ModelRef {
    u8 pad0[0x8];
    Model *model;               /* 0x08 */
    s32 unkC;                   /* 0x0C */
    u8 pad10[0x1C - 0x10];
    s32 unk1C;                  /* 0x1C */
} ModelRef;
#endif

Mtx *MtxPool_Alloc(void *pool, s32 count);
void CMtxF__Multiply(f32 (*mf)[4], void *arg1, void *arg2);
void CMtxF__ClampEntries(f32 (*mf)[4]);
void CMtxF__ToMtx(f32 (*mf)[4], Mtx *m);
void Render_DrawModelFlat(s32, sptr, sptr, s32, Slot24 *, sptr, s32, s32);

void CGameObjectInstance__SubmitShadowModel(CGameObjectInstance *owner, ModelRef *ref, void *arg2, s32 arg3) {
    f32 mf[4][4];
    Model *model;
    Mtx *m;

    if (owner->m_pmtDrawMtxs != NULL) {
        m = MtxPool_Alloc(gScene.m_Heap, 1);
        if (m != NULL) {
            model = ref->model;
            CMtxF__Multiply(mf, arg2, model->unk1CC);
            CMtxF__ClampEntries(mf);
            CMtxF__ToMtx(mf, m);
            {
                Gfx *g = gDList++;
                g->w0 = 0xDA380007;
                g->w1 = GBI_ADDR(m);
            }
            Render_DrawModelFlat((u8)arg3, ref->unkC, (sptr) owner->m_pmtDrawMtxs, 1, &owner->m_Lights[gEvenOdd], 0, ref->unk1C,
                          owner->m_AI.m_nTexture);
            {
                Gfx *g = gDList++;
                g->w0 = 0xDA380007;
                g->w1 = GBI_ADDR(&model->mtx[gEvenOdd]);
            }
        }
    }
}
