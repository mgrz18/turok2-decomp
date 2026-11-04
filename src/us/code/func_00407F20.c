#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"

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
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ void *camera;
    /* 0x520 */ u8 pad520[0x9D0 - 0x520];
    /* 0x9D0 */ s32 bossKilled[5];
    /* 0x9E4 */ u8 pad9E4[0xAAC - 0x9E4];
    /* 0xAAC */ f32 unkAAC;
} Player;

extern Player *D_80119890;     /* the current player */
extern u8 D_800F7078[];
extern u8 D_800B6660[];
extern u8 D_801100F0[];
extern CListedInstance *D_8010FEB4[]; /* the actor list head */
extern s32 D_80110048;         /* level id */

Player *func_002532A8(CPlayerList *);
void func_0027FC2C(void *hud, void *camera, u8 *msg);
void func_0027580C(s32 sound);
void func_002535E4(CPlayerList *list, f32 value);
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

/* The boss is beaten: message, sound, its death animation, the helpers stop. */
void func_00407F20(BossFight *fight) {
    CGame *game = &D_80119870;
    Player *player;
    CListedInstance *a;
    GameSettings *flags;

    fight->state = 1;
    player = func_002532A8(&game->m_PlayerList);
    func_0027FC2C(game->m_CameraPool, player->camera, D_800B6660);
    func_0027580C(0x168);
    func_002535E4(&game->m_PlayerList, 900.0f);
    setAnim(fight->obj, 0x5337);
    func_00225D0C(D_800F7078);
    D_80119890->unkAAC = 450.0f;
    for (a = D_8010FEB4[0]; a != NULL; a = a->m_pNext) {
        if (a->m_Instance.header.m_pIntelligence->m_Type == 1 && a->m_Instance.m_AI.m_Health != 0) {
            a->m_Instance.m_AI.m_pPathTarget = NULL;
            a->m_Instance.m_AI.m_pSightTarget = NULL;
            a->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            a->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
    fight->timer = 60.0f;
    func_0027AD98(D_801100F0);
    func_0027ADA0(D_801100F0);
    switch (D_80110048) {
    case 0x1BBC:
        player->bossKilled[0] = 1;
        flags = &D_8011AAD8;
        flags->unlockedCheats |= 0x8000;
        break;
    case 0x1C20:
        player->bossKilled[1] = 1;
        flags = &D_8011AAD8;
        flags->unlockedCheats |= 0x200;
        break;
    case 0x1C84:
        player->bossKilled[2] = 1;
        flags = &D_8011AAD8;
        flags->unlockedCheats |= 0x400;
        break;
    case 0x1CE8:
        player->bossKilled[3] = 1;
        flags = &D_8011AAD8;
        flags->unlockedCheats |= 0x80;
        break;
    case 0x1D4C:
        player->bossKilled[4] = 1;
        flags = &D_8011AAD8;
        flags->unlockedCheats |= 0x40;
        break;
    }
}
