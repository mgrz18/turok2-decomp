#include "common.h"

/* AI attack-wait callback: state 0 counts the delay down; once it has run out
 * (more than 30 frames left means an attack is due) it plays the attack
 * animation for the current mode if the definition allows it. State 1 counts
 * the cool-down and re-picks a region. */

typedef struct { f32 x, y, z; } Vec3;

typedef struct {
    /* 0x00 */ u8 pad0[0x7E];
    /* 0x7E */ u16 attackMask; /* 5 bits per mode */
} ActorInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorInfo info;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ u8 pad10[0x14 - 0x10];
    /* 0x014 */ ActorDef *def;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ u8 pad8[0x34 - 0x8];
    /* 0x034 */ s8 state;
    /* 0x035 */ u8 pad35[0x37 - 0x35];
    /* 0x037 */ s8 waitState;
    /* 0x038 */ u8 pad38[0x44 - 0x38];
    /* 0x044 */ f32 wait;
    /* 0x048 */ u8 pad48[0x94 - 0x48];
    /* 0x094 */ s8 hasTarget;
    /* 0x095 */ u8 targetId;
    /* 0x096 */ s16 region;
    /* 0x098 */ u8 pad98[0x120 - 0x98];
    /* 0x120 */ s32 unk120;
    /* 0x124 */ u8 pad124[0x133 - 0x124];
    /* 0x133 */ s8 modeIndex;
} CAI;

extern f32 D_800B6D28; /* frame delta */
extern Vec3 D_800B06A0;
extern Vec3 D_800B06AC;
extern u8 D_800F7078[];

extern void func_00243414(Actor *actor, CAI *ai, s32 anim);
extern s16 func_002261A8(void *level, s32 id, Vec3 *pos);

static __inline__ s32 canAttack(CAI *ai, ActorInfo *info, s32 state) {
    if (state == 0x20 || state == 0x3D) {
        return 0;
    }
    switch (ai->modeIndex) {
    case 0:
        return (info->attackMask & 0x1F) != 0;
    case 1:
        return (info->attackMask & 0x3E0) != 0;
    case 2:
        return (info->attackMask & 0x7C00) != 0;
    }
    return 0;
}

void func_0025C430(Actor *actor, CAI *ai) {
    s32 state;

    if (ai->unk4 == 0) {
        return;
    }
    state = ai->state;
    if (state == 0) {
        return;
    }
    switch (ai->waitState) {
    case 0:
        if (ai->wait > 30.0f) {
            if (canAttack(ai, &actor->def->info, state)) {
                func_00243414(actor, ai, 0xC);
            }
            D_800B06A0 = D_800B06AC;
            ai->waitState = 1;
        } else if (ai->wait > 0.0f) {
            ai->wait -= D_800B6D28;
            if (ai->wait < 0.0f) {
                ai->wait = 0.0f;
            }
        }
        ai->unk120 = 0;
        break;
    case 1:
        ai->wait -= D_800B6D28;
        if (ai->wait <= 0.0f) {
            ai->wait = 0.0f;
            ai->waitState = 0;
            if (ai->hasTarget) {
                ai->region = func_002261A8(D_800F7078, ai->targetId, &actor->pos);
            }
        }
        break;
    }
}
