#include "common.h"
#include "structs/game_object.h"

/* AI draw callback: draws the actor faded in or out (dying / teleporting states). */

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 model;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 set0;
} DrawParams;

extern s32 D_800AB8B0; /* fade mode */
extern f32 D_800AB8C0[2]; /* fade level, alpha */
extern s32 D_800B6D1C; /* current view */
extern u8 D_801100D4[];

extern void func_0021D928(CGameObjectInstance *actor, CAI *ai, DrawParams *params);
extern void func_0021D7BC(CGameObjectInstance *actor, CAI *ai, DrawParams *params);
extern void func_0020B258(s32 model, s32 matrix, s32 segment, void *lights, s32 textures, s32 lod, s32 pass);
extern void func_0020BBE4(void *arg0, s32 renderMode, s32 model, s32 matrix, s32 projected, void *lights,
                          s32 segment, s32 group, s32 arg8);

void func_0025BA80(CGameObjectInstance *actor, CAI *ai, DrawParams *params) {
    u8 alpha;

    if (ai->m_Mode == 0x20) {
        func_0021D928(actor, ai, params);
        func_0020B258(params->model, actor->m_pmtDrawMtxs, 1, actor->m_Lights[D_800B6D1C], 0, ((s8 *)params)[0x1B], -1);
        return;
    }
    if (ai->m_VarF32[10] > 0.0f) {
        if (ai->m_Mode == 0x19) {
            alpha = ai->m_AnimRepeatedCount * 255 / 4;
        } else {
            alpha = 255;
            if (ai->m_VarF32[10] < 30.0f) {
                alpha = ai->m_VarF32[10] * 8.5f;
            }
        }
        if (alpha != 255) {
            D_800AB8B0 = 1;
            D_800AB8C0[0] = 255 - alpha;
            func_0021D928(actor, ai, params);
        }
        if (alpha != 0) {
            D_800AB8C0[0] = alpha;
            D_800AB8B0 = 1;
            func_0020BBE4(D_801100D4, -1, params->model, actor->m_pmtDrawMtxs, 1, actor->m_Lights[D_800B6D1C], 0,
                          params->set0, actor->m_AI.m_nTexture);
        }
        D_800AB8B0 = 0;
    } else {
        func_0021D7BC(actor, ai, params);
    }
}
