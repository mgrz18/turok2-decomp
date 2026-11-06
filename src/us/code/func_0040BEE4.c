#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/light.h"
#include "structs/model.h"

/* Draw callback of the module's actors: draws each body part (anim group) whose slot is active in the
 * module object's mask, refreshing the slot's state first unless a cinema path is playing. */

typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ s32 partMask;
} ModuleObj;

extern s32 D_800B6D1C; /* current view */
extern void CGameObjectInstance__DrawShadow(CGameObjectInstance *actor, void *ai, CDrawArgs *req);
extern s32 Cinema_PathPlaying(void);
extern void ScriptAnimator_Apply(void *slotData);
extern void ScreenTint_UpdateMode(void);
extern void Render_DrawModel(s32 hModel, s32 model, s32 one, CLights1 *slot, s32 zero, s32 index, s32 second);

void MotherBoss_Draw(CGameObjectInstance *actor, void *ai, CDrawArgs *req) {
    MotherBossState *state;
    u8 *data;
    s32 mask;
    s32 i;
    s32 one;

    i = 0;
    one = 1;
    state = &D_8012FB70;
    mask = ((ModuleObj *)state->obj)->partMask;
    data = (u8 *) &state->slots[0].anim;
    do {
        if (mask & (one << i)) {
            req->m_nModel = i;
            req->m_nShadowModel = i;
            CGameObjectInstance__DrawShadow(actor, ai, req);
            if (!Cinema_PathPlaying()) {
                ScriptAnimator_Apply(data);
                ScreenTint_UpdateMode();
            }
            Render_DrawModel(req->m_hModel, actor->m_pmtDrawMtxs, 1, (CLights1 *) actor->m_Lights[D_800B6D1C], 0, i, actor->m_AI.m_nTexture);
            D_800AB8B0.m_Mode = 0;
        }
        i++;
        data += sizeof(BossWeakSpot);
    } while (i < 15);
}
