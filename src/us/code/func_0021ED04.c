#include "common.h"
#include "gfx.h"
#include "structs/gbi.h"
#include "structs/game_object.h"
#include "globals/scene.h"

typedef struct Slot24 {
    s32 w[6];
} Slot24;

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

extern s32 gEvenOdd;          /* frame buffer index */

Mtx *MtxPool_Alloc(void *pool, s32 count);
void CMtxF__Multiply(f32 (*mf)[4], void *arg1, void *arg2);
void CMtxF__ClampEntries(f32 (*mf)[4]);
void CMtxF__ToMtx(f32 (*mf)[4], Mtx *m);
void Render_DrawModelFlat(s32, s32, s32, s32, Slot24 *, s32, s32, s32);

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
                g->w1 = (u32)m;
            }
            Render_DrawModelFlat((u8)arg3, ref->unkC, (s32) owner->m_pmtDrawMtxs, 1, (Slot24 *) owner->m_Lights[gEvenOdd], 0, ref->unk1C,
                          owner->m_AI.m_nTexture);
            {
                Gfx *g = gDList++;
                g->w0 = 0xDA380007;
                g->w1 = (u32)&model->mtx[gEvenOdd];
            }
        }
    }
}
