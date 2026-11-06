#include "common.h"
#include "structs/game_object.h"

/* An effect's parameters, passed by value. */
typedef struct {
    /* 0x0 */ s16 upper;
    /* 0x2 */ s16 lower;
    /* 0x4 */ u8 b[4];
} __attribute__((aligned(4))) EffectParams;

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void ImpactEvent_Send(CGameObjectInstance *src, CGameObjectInstance *dst, s32 effect, s32 id,
                          CVector3 pos, EffectParams params);

extern s32 D_800F1F50[];
extern s32 D_8012F988;          /* parts left (mode 1) */
extern s32 D_8012F990;          /* parts left (mode 2) */

void Primagen_HandleAnimEvent(s32 arg0, CGameObjectInstance *obj, s32 arg2, CVector3 pos, s32 event) {
    CAITargets t;
    EffectParams params;
    f32 size = 10.0f; /* set before the calls: the ROM multiplies by 256 at run time */

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    if (t.m_Sight.m_pTarget == NULL) {
        return;
    }
    if (event == 2 && t.m_Sight.m_Dist <= 409.59998f && D_8012F990 > 0) {
        params.upper = 0xA00;
        params.lower = 0x400;
        params.b[0] = 0;
        params.b[1] = 0;
        params.b[2] = 0;
        params.b[3] = 0;
        ImpactEvent_Send(obj, obj, 3, -1, t.m_Sight.m_pTarget->header.m_vPos, params);
    }
    if (event == 1 && t.m_Sight.m_Dist <= 716.8f && D_8012F988 > 0) {
        params.upper = size * 256.0f;
        params.lower = 0x800;
        params.b[0] = 0;
        params.b[1] = 0;
        params.b[2] = 0;
        params.b[3] = 0;
        ImpactEvent_Send(obj, obj, 3, -1, t.m_Sight.m_pTarget->header.m_vPos, params);
    }
}
