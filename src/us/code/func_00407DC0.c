#include "common.h"
#include "structs/game_object.h"
#include "structs/light.h"
#include "structs/model.h"
#include "globals/boss.h"
#include "globals/engine.h"
#include "protos/game.h"

typedef struct {
    u8 data[0x64];
} Slot00407DC0;

#ifdef TARGET_PORT
typedef struct Boss00407DC0 {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Boss00407DC0(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_slotMask[T2_HOFF_Boss00407DC0(m_AI.m_VisBits)]; s32 slotMask; }; /* N64 0x14C */
        struct { u8 _hp_unk174[T2_HOFF_Boss00407DC0(m_AI.m_Mode)]; s8 unk174; }; /* N64 0x174 */
    };
} Boss00407DC0;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ s32 slotMask;
    /* 0x150 */ u8 pad150[0x174 - 0x150];
    /* 0x174 */ s8 unk174;
} Boss00407DC0;
#endif

s32 Cinema_PathPlaying(void);
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void ScriptAnimator_Apply(Slot00407DC0 *slot);
void ScreenTint_UpdateMode(void);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

/* Draws each of the boss's 8 parts present in its mask; part 1 shows mesh 8 in state 29. */
void Primagen_Draw(u8 *actor, u8 *state, CDrawArgs *req) {
    Boss00407DC0 *boss = gLevelScript.actor;
    s32 mask;
    s32 i;

    mask = boss->slotMask;
    for (i = 0; i < 8; i++) {
        if (hasBit(mask, 1 << i)) {
            if (i == 1 && boss->unk174 == 29) {
                req->m_nModel = 8;
                req->m_nShadowModel = 8;
            } else {
                req->m_nModel = i;
                req->m_nShadowModel = i;
            }
            CGameObjectInstance__DrawShadow(actor, state, req);
            if (!Cinema_PathPlaying()) {
                ScriptAnimator_Apply(&gLevelScript.anims[i]);
                ScreenTint_UpdateMode();
            }
#ifdef TARGET_PORT
            /* the actor's draw matrices, lights and texture by member (N64 0x98, 0x110, 0x151) */
            Render_DrawModel(req->m_hModel, ((CGameObjectInstance *) actor)->m_pmtDrawMtxs, 1,
                             (char *) &((CGameObjectInstance *) actor)->m_Lights[gEvenOdd], 0, req->m_nModel,
                             ((CGameObjectInstance *) actor)->m_AI.m_nTexture);
#else
            Render_DrawModel(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0,
                          req->m_nModel, *(s8 *) (actor + 0x151));
#endif
            gBlendState = 0;
        }
    }
}
