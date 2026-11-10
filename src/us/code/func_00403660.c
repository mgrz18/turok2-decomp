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

#ifdef TARGET_PORT
typedef struct Boss00403660 {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Boss00403660(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_unk54[T2_HOFF_Boss00403660(m_CollisionYOffset)]; s32 unk54; }; /* N64 0x54 */
        struct { u8 _hp_slotMask[T2_HOFF_Boss00403660(m_AI.m_VisBits)]; s32 slotMask; }; /* N64 0x14C */
        struct { u8 _hp_unk174[T2_HOFF_Boss00403660(m_AI.m_Mode)]; s8 unk174; }; /* N64 0x174 */
    };
} Boss00403660;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ s32 unk54;
    /* 0x058 */ u8 pad1[0x14C - 0x58];
    /* 0x14C */ s32 slotMask;
    /* 0x150 */ u8 pad2[0x174 - 0x150];
    /* 0x174 */ s8 unk174;
} Boss00403660;
#endif

void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void ScriptAnimator_Apply(Slot00403660 *slot);
void ScreenTint_UpdateMode(void);

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
    if (boss->unk174 != 2) {
        boss->unk54 = 0;
    }
}
