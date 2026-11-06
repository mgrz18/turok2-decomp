#include "common.h"
#include "structs/boss.h"
#include "structs/light.h"
#include "structs/model.h"

/* A weak spot seen from its animator: walking BossWeakSpot pointers and
 * taking &slot->anim moves the +4 into the loop and breaks the match. */
typedef struct {
    u8 data[0x68];
} Slot00403660;

typedef struct {
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ s32 unk54;
    /* 0x058 */ u8 pad1[0x14C - 0x58];
    /* 0x14C */ s32 slotMask;
    /* 0x150 */ u8 pad2[0x174 - 0x150];
    /* 0x174 */ s8 unk174;
} Boss00403660;

extern s32 gEvenOdd;
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void ScriptAnimator_Apply(Slot00403660 *slot);
void ScreenTint_UpdateMode(void);
void Render_DrawModel(s32 hModel, s32 model, s32 one, CLights1 *slot, s32 zero, s32 index, s32 second);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

void MantidQueen_Draw(u8 *actor, u8 *state, CDrawArgs *req) {
    WaveBossState *s;
    Boss00403660 *boss;
    s32 mask;
    Slot00403660 *slot;
    s32 i;
    s32 bit;

    i = 0;
    bit = 1;
    s = &gMantidQueen;
    boss = (Boss00403660 *) s->actor;
    mask = boss->slotMask;
    slot = (Slot00403660 *) &s->spots[0].anim;
    do {
        if (hasBit(mask, bit << i)) {
            req->m_nModel = i;
            req->m_nShadowModel = i;
            CGameObjectInstance__DrawShadow(actor, state, req);
            ScriptAnimator_Apply(slot);
            ScreenTint_UpdateMode();
            Render_DrawModel(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0, i,
                          *(s8 *) (actor + 0x151));
            gBlendState.m_Mode = 0;
        }
        i++;
        slot++;
    } while (i < 5);
    if (boss->unk174 != 2) {
        boss->unk54 = 0;
    }
}
