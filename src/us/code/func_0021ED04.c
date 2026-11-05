#include "common.h"
#include "gfx.h"
#include "structs/gbi.h"
#include "structs/game_object.h"

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

extern s32 D_800B6D1C;          /* frame buffer index */
extern u8 D_800F7148[];

Mtx *func_00206950(void *pool, s32 count);
void func_0020C890(f32 (*mf)[4], void *arg1, void *arg2);
void func_0020F7EC(f32 (*mf)[4]);
void func_0020CC94(f32 (*mf)[4], Mtx *m);
void func_0020AF08(s32, s32, s32, s32, Slot24 *, s32, s32, s32);

void func_0021ED04(CGameObjectInstance *owner, ModelRef *ref, void *arg2, s32 arg3) {
    f32 mf[4][4];
    Model *model;
    Mtx *m;

    if (owner->m_pmtDrawMtxs != NULL) {
        m = func_00206950(D_800F7148, 1);
        if (m != NULL) {
            model = ref->model;
            func_0020C890(mf, arg2, model->unk1CC);
            func_0020F7EC(mf);
            func_0020CC94(mf, m);
            {
                Gfx *g = D_800D8E24++;
                g->w0 = 0xDA380007;
                g->w1 = (u32)m;
            }
            func_0020AF08((u8)arg3, ref->unkC, (s32) owner->m_pmtDrawMtxs, 1, (Slot24 *) owner->m_Lights[D_800B6D1C], 0, ref->unk1C,
                          owner->m_AI.m_nTexture);
            {
                Gfx *g = D_800D8E24++;
                g->w0 = 0xDA380007;
                g->w1 = (u32)&model->mtx[D_800B6D1C];
            }
        }
    }
}
