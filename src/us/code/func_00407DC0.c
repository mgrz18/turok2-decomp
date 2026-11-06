#include "common.h"
#include "structs/light.h"
#include "structs/model.h"
#include "globals/boss.h"
#include "globals/engine.h"

typedef struct {
    u8 data[0x64];
} Slot00407DC0;

typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ s32 slotMask;
    /* 0x150 */ u8 pad150[0x174 - 0x150];
    /* 0x174 */ s8 unk174;
} Boss00407DC0;

s32 Cinema_PathPlaying(void);
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void ScriptAnimator_Apply(Slot00407DC0 *slot);
void ScreenTint_UpdateMode(void);
void Render_DrawModel(s32 hModel, s32 model, s32 one, CLights1 *slot, s32 zero, s32 index, s32 second);

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
            Render_DrawModel(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0,
                          req->m_nModel, *(s8 *) (actor + 0x151));
            gBlendState.m_Mode = 0;
        }
    }
}
