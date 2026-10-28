#include "common.h"

/* AI combat update: turns toward the target, then picks a reaction (taunt,
 * the 0x11F4-range anims, a dodge or a special melee for type 0x259). */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 padC[0x44 - 0xC];
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ u16 tauntMask; /* 5 bits per mode */
} ActorInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorInfo info;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xB8 - 0x18];
    /* 0x0B8 */ u16 type;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 length;
} Anim;

typedef struct {
    /* 0x000 */ u32 flags;
    /* 0x004 */ u8 pad4[0x30 - 0x4];
    /* 0x030 */ Anim *anim;
    /* 0x034 */ u8 state;
    /* 0x035 */ u8 prevState;
    /* 0x036 */ u8 unk36;
    /* 0x037 */ u8 pad37[0xC0 - 0x37];
    /* 0x0C0 */ f32 animTime;
    /* 0x0C4 */ u16 animId;
    /* 0x0C6 */ u8 padC6;
    /* 0x0C7 */ s8 hits;
    /* 0x0C8 */ u8 skill;
    /* 0x0C9 */ u8 padC9[0x130 - 0xC9];
    /* 0x130 */ s8 mode[3];
} CAI;

extern void func_00246108(Actor *actor, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(Actor *actor, CAI *ai, f32 angle, f32 dist);
extern void func_00243414(Actor *actor, CAI *ai, s32 anim);
extern void func_0025D474(Actor *actor, CAI *ai);
extern s32 func_0025D030(Actor *actor, CAI *ai, Targets *t);
extern void func_0025D308(Actor *actor, CAI *ai);

static __inline__ s8 canTaunt(CAI *ai, ActorInfo *info) {
    if (ai->mode[0] && (info->tauntMask & 0x1F)) {
        return 1;
    }
    if (ai->mode[1] && (info->tauntMask & 0x3E0)) {
        return 1;
    }
    if (ai->mode[2] && (info->tauntMask & 0x7C00)) {
        return 1;
    }
    return 0;
}

void func_0025DBD8(Actor *actor, CAI *ai) {
    Targets t;
    f32 angle;
    f32 skill;

    func_00246108(actor, ai, &t, 0, 1);
    angle = 0.261799425f;
    if (ai->unk36 == 1 || ai->unk36 == 2) {
        angle = 1.0471977f;
    }
    func_00245BAC(actor, ai, angle, t.a.unk8);
    if ((ai->flags & 0x4000000) && canTaunt(ai, &actor->def->info)) {
        func_00243414(actor, ai, 0x13);
        ai->flags |= 0x4000000;
        ai->prevState = ai->state;
        ai->animTime = ai->anim->length;
    } else if (ai->animId >= 4500 && ai->animId < 4555) {
        skill = ai->skill;
        skill = skill * (1.0f / 255);
        skill = skill * 5.0f + 1.0f; /* hits needed: 1..6 */
        if (ai->hits >= (s32) skill) {
            func_00243414(actor, ai, 0xA);
        }
    } else if (ai->hits != 0) {
        func_0025D474(actor, ai);
    }
    if (actor->type == 0x259 && func_0025D030(actor, ai, &t) == 0xE) {
        func_00243414(actor, ai, 0xE);
    }
    func_0025D308(actor, ai);
}
