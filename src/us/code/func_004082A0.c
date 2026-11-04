#include "common.h"
#include "structs/game.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 *def;
} BossObj;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ s32 time;
    /* 0x44 */ s16 maxHelpers;
    /* 0x46 */ s16 kills;
} BossDef;

typedef struct {
    /* 0x00 */ BossObj *obj;
    /* 0x04 */ BossDef *def;
    /* 0x08 */ s32 state;
    /* 0x0C */ f32 timer;
    /* 0x10 */ s32 count;
    /* 0x14 */ s32 warned;
} BossFight;


typedef struct Actor {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ s32 *type;
    /* 0x018 */ u8 pad18[0x140 - 0x18];
    /* 0x140 */ s32 flags;
    /* 0x144 */ u8 pad144[0x1B8 - 0x144];
    /* 0x1B8 */ s32 dead;
    /* 0x1BC */ u8 pad1BC[4];
    /* 0x1C0 */ void *target;
    /* 0x1C4 */ u8 pad1C4[4];
    /* 0x1C8 */ void *lookAt;
    /* 0x1CC */ u8 pad1CC[0x28C - 0x1CC];
    /* 0x28C */ struct Actor *next;
} Actor;

extern void *D_80119890;
extern s32 D_800F7078;
extern f32 D_800B6D28;
extern u8 D_800B6684[];
extern u8 D_801100F0[];
extern Actor *D_8010FEB4;

void *func_002532A8(void *);
void func_00281610(void *, s32, void *, f32);
void func_0027580C(s32);
void func_0027AD88(void *, s32);
void func_0027AD90(void *, s32);
s8 func_00219F74(void *, s32, s32);
void func_0040813C(BossFight *);
void func_00407F20(BossFight *);

static __inline__ s32 toInt(f32 f) {
    s32 value;

    value = 0;
    if (!(f < 0.0f)) {
        value = f;
    }
    return value;
}

void func_004082A0(BossFight *fight) {
    f32 timer;
    u8 *bar;
    void *player;
    s32 left;
    s32 left2;
    s32 n;
    Actor *actor;
    u8 *obj;
    u8 *ai;

    if (D_80119870.m_Settings.mode != 13 || D_800F7078 != 4) {
        return;
    }
    if ((f32) (*(s32 *) (fight->obj->def + 0x54) * 15) <= fight->timer) {
        func_00281610(D_80119870.m_CameraPool, *(s32 *) ((u8 *) func_002532A8(&D_80119870.m_PlayerList) + 0x51C), D_800B6684, 4.0f);
        func_0027580C(0x182);
    }
    bar = D_801100F0;
    func_0027AD88(bar, toInt(fight->timer * (1.0f / 15) / (f32) fight->def->time * 100.0f));
    func_0027AD90(bar, toInt((f32) (fight->def->kills - fight->count) / (f32) fight->def->kills * 100.0f));
    if (fight->state == 0) {
        if (fight->timer < 450.0f && fight->warned != 1) {
            obj = (u8 *) fight->obj;
            ai = obj + 0x140;
            ai[0xC6] = func_00219F74(obj, 0x5335, -1);
            *(s16 *) (ai + 0xC4) = 0x5335;
            *(s32 *) (obj + 0x140) &= ~1;
            ai[0xC7] = 0;
            fight->warned = 1;
        }
        timer = fight->timer - D_800B6D28;
        left = fight->def->kills - fight->count;
        left2 = left;
        fight->timer = timer;
        if (timer <= 0.0f) {
            if (left > 0) {
                func_0040813C(fight);
            } else {
                func_00407F20(fight);
            }
        } else if (left2 <= 0) {
            func_00407F20(fight);
        } else {
            fight->state = 0;
        }
    }
    if (fight->state != 1) {
        actor = D_8010FEB4;
        player = D_80119890;
        n = 0;
        for (; actor != NULL; actor = actor->next) {
            if (*actor->type == 1 && actor->dead == 0) {
                if (fight->state == 0) {
                    if (n < fight->def->maxHelpers) {
                        /* the first helpers keep chasing the player */
                        n++;
                        actor->lookAt = player;
                        actor->target = player;
                        actor->flags |= 0x20000000;
                        actor->flags |= 0x40000000;
                    } else {
                        actor->target = fight->obj;
                        actor->lookAt = fight->obj;
                        actor->flags |= 0x20000000;
                        actor->flags |= 0x40000000;
                    }
                } else if (actor->target == fight->obj) {
                    actor->lookAt = player;
                    actor->target = player;
                    actor->flags |= 0x20000000;
                    actor->flags |= 0x40000000;
                }
            }
        }
    }
}
