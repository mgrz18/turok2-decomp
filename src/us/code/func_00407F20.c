#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC4];
    /* 0xC4 */ s16 animId;
    /* 0xC6 */ s8 anim;
    /* 0xC7 */ s8 unkC7;
} BossAI;

typedef struct Actor {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ s32 *type;
    /* 0x018 */ u8 pad18[0x140 - 0x18];
    /* 0x140 */ s32 flags;         /* the AI's flags, ai at 0x140 */
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x1C0 - 0x148];
    /* 0x1C0 */ void *target;
    /* 0x1C4 */ u8 pad1C4[4];
    /* 0x1C8 */ void *lookAt;
    /* 0x1CC */ u8 pad1CC[0x28C - 0x1CC];
    /* 0x28C */ struct Actor *next;
} Actor;

typedef struct {
    /* 0x00 */ Actor *obj;
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

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ void *camera;
    /* 0x520 */ u8 pad520[0x9D0 - 0x520];
    /* 0x9D0 */ s32 bossKilled[5];
    /* 0x9E4 */ u8 pad9E4[0xAAC - 0x9E4];
    /* 0xAAC */ f32 unkAAC;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u32 flags;
} GameFlags;

extern Game D_80119870;
extern Player *D_80119890;     /* the current player */
extern GameFlags D_8011AAD8;
extern u8 D_800F7078[];
extern u8 D_800B6660[];
extern u8 D_801100F0[];
extern Actor *D_8010FEB4[]; /* the actor list head */
extern s32 D_80110048;         /* level id */

Player *func_002532A8(Game *);
void func_0027FC2C(void *hud, void *camera, u8 *msg);
void func_0027580C(s32 sound);
void func_002535E4(Game *game, f32 value);
s8 func_00219F74(Actor *actor, s32 id, s32);
void func_00225D0C(void *world);
void func_0027AD98(void *);
void func_0027ADA0(void *);

static __inline__ void setAnim(Actor *actor, s32 id) {
    BossAI *ai;
    s8 anim = func_00219F74(actor, id, -1);

    ai = (BossAI *) &actor->flags;
    ai->anim = anim;
    ai->animId = id;
    actor->flags &= ~1;
    ai->unkC7 = 0;
}

/* The boss is beaten: message, sound, its death animation, the helpers stop. */
void func_00407F20(BossFight *fight) {
    Game *game = &D_80119870;
    Player *player;
    Actor *a;
    GameFlags *flags;

    fight->state = 1;
    player = func_002532A8(game);
    func_0027FC2C(game->hud, player->camera, D_800B6660);
    func_0027580C(0x168);
    func_002535E4(game, 900.0f);
    setAnim(fight->obj, 0x5337);
    func_00225D0C(D_800F7078);
    D_80119890->unkAAC = 450.0f;
    for (a = D_8010FEB4[0]; a != NULL; a = a->next) {
        if (*a->type == 1 && a->health != 0) {
            a->lookAt = NULL;
            a->target = NULL;
            a->flags |= 0x20000000;
            a->flags |= 0x40000000;
        }
    }
    fight->timer = 60.0f;
    func_0027AD98(D_801100F0);
    func_0027ADA0(D_801100F0);
    switch (D_80110048) {
    case 0x1BBC:
        player->bossKilled[0] = 1;
        flags = &D_8011AAD8;
        flags->flags |= 0x8000;
        break;
    case 0x1C20:
        player->bossKilled[1] = 1;
        flags = &D_8011AAD8;
        flags->flags |= 0x200;
        break;
    case 0x1C84:
        player->bossKilled[2] = 1;
        flags = &D_8011AAD8;
        flags->flags |= 0x400;
        break;
    case 0x1CE8:
        player->bossKilled[3] = 1;
        flags = &D_8011AAD8;
        flags->flags |= 0x80;
        break;
    case 0x1D4C:
        player->bossKilled[4] = 1;
        flags = &D_8011AAD8;
        flags->flags |= 0x40;
        break;
    }
}
