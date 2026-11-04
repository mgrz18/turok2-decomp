#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC4];
    /* 0xC4 */ s16 animId;
    /* 0xC6 */ s8 anim;
    /* 0xC7 */ s8 unkC7;
} BossAI;

typedef struct {
    /* 0x00 */ CListedInstance *obj;
    /* 0x04 */ void *def;
    /* 0x08 */ s32 state;
    /* 0x0C */ f32 timer;
    /* 0x10 */ s32 count;
    /* 0x14 */ s32 warned;
} BossFight;

typedef struct {
    /* 0x0000 */ u8 pad0[0x40];
    /* 0x0040 */ u8 hud[0x13D8];
    /* 0x1418 */ s32 mode;
} Game;

extern Game D_80119870;
extern CPlayer *D_80119890;     /* the current player */
extern u8 D_800F7078[];
extern u8 D_800B6674[];
extern u8 D_801100F0[];
extern CListedInstance *D_8010FEB4[]; /* the actor list head */

CPlayer *func_002532A8(Game *);
void func_0027FC2C(void *hud, void *camera, u8 *msg);
void func_0027580C(s32 sound);
s8 func_00219F74(CListedInstance *actor, s32 id, s32);
void func_00225D0C(void *world);
void func_0027AD98(void *);
void func_0027ADA0(void *);

static __inline__ void setAnim(CListedInstance *actor, s32 id) {
    BossAI *ai;
    s8 anim = func_00219F74(actor, id, -1);

    ai = (BossAI *) &actor->m_Instance.m_AI.m_dwFlags;
    ai->anim = anim;
    ai->animId = id;
    actor->m_Instance.m_AI.m_dwFlags &= ~1;
    ai->unkC7 = 0;
}

/* The fight starts: message, the boss's intro animation, and the helpers
 * (those not dead and, unless the fight is over, targeting the boss) turn on
 * the current player. */
void func_0040813C(BossFight *fight) {
    Game *game = &D_80119870;
    CPlayer *player;
    CListedInstance *a;
    CPlayer *current;

    player = func_002532A8(game);
    func_0027FC2C(game->hud, player->m_pCamera, D_800B6674);
    player->m_InvincibilityTime = 900.0f;
    fight->state = 2;
    fight->warned = 2;
    setAnim(fight->obj, 0x5336);
    func_0027AD98(D_801100F0);
    func_0027ADA0(D_801100F0);
    func_00225D0C(D_800F7078);
    if (fight->state != 1) {
        current = D_80119890;
        for (a = D_8010FEB4[0]; a != NULL; a = a->m_pNext) {
            if (a->m_Instance.header.m_pIntelligence->m_Type == 1 && a->m_Instance.m_AI.m_pTargetOverride == 0 && (fight->state == 0 || a->m_Instance.m_AI.m_pSightTarget == fight->obj)) {
                a->m_Instance.m_AI.m_pPathTarget = current;
                a->m_Instance.m_AI.m_pSightTarget = current;
                a->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                a->m_Instance.m_AI.m_dwFlags |= 0x40000000;
            }
        }
    }
}
