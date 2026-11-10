#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/light.h"
#include "structs/model.h"
#include "globals/boss.h"
#include "globals/engine.h"
#include "protos/game.h"

/* Draw callback of the module's actors: draws each body part (anim group) whose slot is active in the
 * module object's mask, refreshing the slot's state first unless a cinema path is playing. */

#ifdef TARGET_PORT
typedef struct ModuleObj {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ModuleObj(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_partMask[T2_HOFF_ModuleObj(m_AI.m_VisBits)]; s32 partMask; }; /* N64 0x14C */
    };
} ModuleObj;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ s32 partMask;
} ModuleObj;
#endif

extern void CGameObjectInstance__DrawShadow(CGameObjectInstance *actor, void *ai, CDrawArgs *req);
extern s32 Cinema_PathPlaying(void);
extern void ScriptAnimator_Apply(void *slotData);
extern void ScreenTint_UpdateMode(void);

void MotherBoss_Draw(CGameObjectInstance *actor, void *ai, CDrawArgs *req) {
    MotherBossState *state;
    u8 *data;
    s32 mask;
    s32 i;
    s32 one;

    i = 0;
    one = 1;
    state = &gMotherBoss;
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
            Render_DrawModel(req->m_hModel, actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0, i, actor->m_AI.m_nTexture);
            gBlendState = 0;
        }
        i++;
        data += sizeof(BossWeakSpot);
    } while (i < 15);
}
