#include "common.h"
#include "structs/vector.h"
#include "structs/game.h"

/* Boss fight AI tick: runs the fight phase (timer and helpers, then the
 * random stun of a live helper), and once the boss has waited long enough
 * outside the fight starts the level's boss music. */

typedef struct Actor {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x14 - 0x10];
    /* 0x014 */ s32 *type;
    /* 0x018 */ u8 pad18[0x140 - 0x18];
    /* 0x140 */ u8 ai[0x144 - 0x140];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x28C - 0x148];
    /* 0x28C */ struct Actor *next;
} Actor;

typedef struct {
    /* 0x00 */ void *obj;
    /* 0x04 */ void *def;
    /* 0x08 */ s32 state;
    /* 0x0C */ f32 timer;
} BossFight;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 idle;
    /* 0x44 */ u8 pad44[0xF4 - 0x44];
    /* 0xF4 */ void *cbF4;
} CAI;

typedef struct {
    /* 0x000 */ u8 pad0[0x9E4];
    /* 0x9E4 */ s32 metBoss1;
    /* 0x9E8 */ s32 metBoss2;
    /* 0x9EC */ u8 pad9EC[0xA28 - 0x9EC];
    /* 0xA28 */ s32 unkA28;
} Player;

extern BossFight D_8012FB50;
extern Actor *D_8010FEB4;
extern f32 D_800B6D28;
extern s32 D_80110048;

extern void func_004089E0();
extern void func_004082A0(BossFight *fight);
extern f32 func_0021149C(f32 range);
extern void func_00246690(Actor *actor, s32 sound, CVector3 pos, s32 arg);
extern void func_00245828(void *out, s32 a, s32 b, f32 c, s32 d, s32 e);
extern void func_0025B2D4(Actor *actor, void *ai, void *anim);
extern Player *func_002532A8(void *players);
extern void func_00248BA8(Player *player, s32 track, s32 a, s32 b);

static __inline__ s32 helperAlive(void) {
    Actor *actor;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
        if (*actor->type == 1 && actor->health != 0) {
            return 1;
        }
    }
    return 0;
}

static __inline__ void stunHelper(void) {
    u8 anim[0x18];
    Actor *actor;

    if (D_8012FB50.timer > 0.0f) {
        D_8012FB50.timer -= D_800B6D28;
    } else if (!(func_0021149C(100.0f) > 25.0f)) {
        for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
            if (*actor->type == 1 && actor->health != 0) {
                func_00246690(actor, 0x17C, actor->pos, 0);
                func_00245828(anim, 0, 0xFF00, 51.199997f, 0x100000, 0);
                func_0025B2D4(actor, actor->ai, anim);
                break;
            }
        }
    }
}

void func_004085A0(void *obj, CAI *ai) {
    BossFight *fight = &D_8012FB50;
    Player *player;
    s32 track;

    if (ai->cbF4 != func_004089E0) {
        ai->cbF4 = func_004089E0;
    }
    switch (fight->state) {
    case 0:
        func_004082A0(fight);
        break;
    case 1:
        stunHelper();
        break;
    case 2:
        helperAlive();
        break;
    }
    if (fight->state == 0) {
        ai->idle = 0.0f;
        return;
    }
    if (fight->state == 1) {
        if (helperAlive()) {
            ai->idle = 0.0f;
            return;
        }
        if (ai->idle < 90.0f) {
            return;
        }
    } else if (ai->idle < 75.0f) {
        return;
    }
    player = func_002532A8(&D_80119870.m_PlayerList);
    if (player == NULL) {
        return;
    }
    switch (D_80110048) {
    case 0x1BBC:
        track = 0x7CF;
        break;
    case 0x1C20:
        track = 0xBB7;
        break;
    case 0x1C84:
        track = 0xF9F;
        break;
    case 0x1CE8:
        track = 0x1CE9;
        if (player->metBoss1) {
            track = 0x1387;
        }
        break;
    case 0x1D4C:
        track = 0x1D4D;
        if (player->metBoss2) {
            track = 0x176F;
        }
        break;
    default:
        track = D_80110048;
        break;
    }
    player->unkA28 = 0;
    func_00248BA8(player, track, 2, 0);
}
