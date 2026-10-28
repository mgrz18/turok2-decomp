#include "common.h"

/* AI init for a family of actors: installs the AI callbacks and seeds the
 * timers/flags from the actor definition. Types 0x44E..0x456 (except 0x44F and
 * 0x452) are left alone. */

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x4C - 0x4];
    /* 0x4C */ s8 mode[2];
    /* 0x4E */ u8 mode2;
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
    /* 0x0BA */ u8 padBA[0xD4 - 0xBA];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ void *anims;
    /* 0x030 */ u8 pad30[0xF0 - 0x30];
    /* 0x0F0 */ void *update;
    /* 0x0F4 */ void *draw;
    /* 0x0F8 */ void *cbF8;
    /* 0x0FC */ void *cbFC;
    /* 0x100 */ void *cb100;
    /* 0x104 */ void *cb104;
    /* 0x108 */ void *cb108;
    /* 0x10C */ s32 unk10C;
    /* 0x110 */ s32 unk110;
    /* 0x114 */ s32 unk114;
    /* 0x118 */ s32 unk118;
    /* 0x11C */ f32 unk11C;
    /* 0x120 */ s32 unk120;
    /* 0x124 */ s32 unk124;
    /* 0x128 */ s32 unk128;
    /* 0x12C */ f32 unk12C;
    /* 0x130 */ s8 mode[2];
    /* 0x132 */ u8 mode2;
    /* 0x133 */ s8 modeIndex;
    /* 0x134 */ s32 unk134;
    /* 0x138 */ f32 unk138;
} CAI;

extern struct { u8 value; } D_8011AAF1;
extern u8 D_800B0A60[];
extern u8 D_800F7078[];

extern void func_0025C5F8();
extern void func_0025C018();
extern void func_0025BA80();
extern void func_0025CAC8();
extern void func_0025EF68();
extern void func_0025B2D4();
extern void func_0025E4B8();
extern void func_00220260(void *mgr, Actor *actor, s32 flag);
extern f32 func_002119FC(f32 lo, f32 hi);

void func_0025BC70(Actor *actor, CAI *ai) {
    ActorInfo *info;
    s32 mode2;
    s32 one; /* the mode indices live in variables: constants do not match */
    s32 two;

    actor->flags |= 0x02000000;
    actor->flags |= 0x04000000;
    info = &actor->def->info;
    if (D_8011AAF1.value == 0) {
        actor->flags |= 0x10000000;
    }
    switch (actor->type) {
    case 0x44E:
    case 0x450:
    case 0x451:
    case 0x453:
    case 0x454:
    case 0x455:
    case 0x456:
        return;
    }
    ai->anims = D_800B0A60;
    ai->update = func_0025C5F8;
    ai->cb104 = func_0025C018;
    ai->draw = func_0025BA80;
    if (actor->type == 0x136) {
        ai->cb100 = NULL;
        ai->cbFC = NULL;
        ai->cbF8 = NULL;
        func_00220260(D_800F7078, actor, 0);
    } else {
        ai->cb100 = func_0025CAC8;
        ai->cbFC = func_0025EF68;
        ai->cbF8 = func_0025B2D4;
    }
    ai->unk10C = 0;
    ai->unk110 = 0;
    ai->unk114 = 0;
    ai->unk118 = 0;
    ai->unk120 = 0;
    ai->unk124 = 0;
    ai->unk128 = 0;
    ai->unk11C = 1.0f;
    ai->unk12C = func_002119FC(60.0f, 120.0f);
    ai->unk134 = 0;
    ai->unk138 = 150.0f;
    ai->mode[0] = info->mode[0];
    one = 1;
    ai->mode[one] = info->mode[one];
    ai->mode2 = mode2 = info->mode2;
    if (ai->mode[0]) {
        ai->modeIndex = 0;
    } else if (ai->mode[one]) {
        ai->modeIndex = one;
    } else {
        two = 2;
        if (mode2) {
            ai->modeIndex = two;
        } else {
            ai->modeIndex = 0;
        }
    }
    ai->cb108 = func_0025E4B8;
    if (!(actor->flags & 0x80000) && (info->flags & 0x1000)) {
        actor->flags &= ~0x2000;
        actor->flags &= ~0x100;
    }
    if (!(info->flags & 0x80000)) {
        actor->flags |= 0x01000000;
    }
}
