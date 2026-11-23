#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/light.h"
#include "structs/model.h"
#include "globals/boss.h"
#include "globals/engine.h"
#include "protos/game.h"

/* A weak spot seen from its animator: walking BossWeakSpot pointers and
 * taking &slot->anim moves the +4 into the loop and breaks the match. */
typedef struct {
#ifdef TARGET_PORT
    u8 data[sizeof(BossWeakSpot)]; /* the host stride of the weak spots (pointers in ScriptAnimator) */
#else
    u8 data[0x68];
#endif
} Slot00403660;


void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void ScriptAnimator_Apply(Slot00403660 *slot);
void ScreenTint_UpdateMode(void);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

void MantidQueen_Draw(u8 *actor, u8 *state, CDrawArgs *req) {
    WaveBossState *s;
    CGameObjectInstance *boss;
    s32 mask;
    Slot00403660 *slot;
    s32 i;
    s32 bit;

    i = 0;
    bit = 1;
    s = &gMantidQueen;
    boss = (CGameObjectInstance *) s->actor;
    mask = boss->m_AI.m_VisBits;
    slot = (Slot00403660 *) &s->spots[0].anim;
    do {
        if (hasBit(mask, bit << i)) {
            req->m_nModel = i;
            req->m_nShadowModel = i;
            CGameObjectInstance__DrawShadow(actor, state, req);
            ScriptAnimator_Apply(slot);
            ScreenTint_UpdateMode();
#ifdef TARGET_PORT
            /* the actor's draw matrices, lights and texture by member (N64 0x98, 0x110, 0x151) */
            Render_DrawModel(req->m_hModel, ((CGameObjectInstance *) actor)->m_pmtDrawMtxs, 1,
                             (char *) &((CGameObjectInstance *) actor)->m_Lights[gEvenOdd], 0, i,
                             ((CGameObjectInstance *) actor)->m_AI.m_nTexture);
#else
            Render_DrawModel(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0, i,
                          *(s8 *) (actor + 0x151));
#endif
            gBlendState = 0;
        }
        i++;
        slot++;
    } while (i < 5);
    if (boss->m_AI.m_Mode != 2) {
        boss->m_CollisionYOffset = 0;
    }
}
