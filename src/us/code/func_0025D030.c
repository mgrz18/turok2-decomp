#include "common.h"

/* Picks an attack for the current mode: every attack the definition enables
 * for this mode and whose range test passes goes into a weighted random
 * chooser; returns the chosen animation, or 0. */

typedef struct {
    /* 0x00 */ u32 kind;
    /* 0x04 */ s32 visible;
    /* 0x08 */ u8 pad8[0x24 - 0x8];
    /* 0x24 */ f32 dist;
    /* 0x28 */ u8 pad28[0x44 - 0x28];
} Target;

typedef struct {
    /* 0x00 */ Target ground;
    /* 0x44 */ Target air;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 meleeRange;
    /* 0x3C */ f32 rangeA;
    /* 0x40 */ f32 rangeB;
    /* 0x44 */ f32 rangeC;
    /* 0x48 */ f32 rangeD;
    /* 0x4C */ u8 pad4C[0x6A - 0x4C];
    /* 0x6A */ u16 meleeMask; /* 5 bits per mode */
    /* 0x6C */ u16 maskA;
    /* 0x6E */ u16 maskB;
    /* 0x70 */ u16 maskC;
    /* 0x72 */ u16 maskD;
} ActorInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorInfo info;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ f32 cooldown;
    /* 0x124 */ u8 pad124[0x133 - 0x124];
    /* 0x133 */ s8 modeIndex;
} CAI;

typedef struct {
    /* 0x00 */ s16 count;
    /* 0x02 */ u8 pad2[0x68 - 0x2];
} Chooser;

extern void func_0026D5DC(Chooser *c);
extern void func_0026D5E4(Chooser *c, s32 id, s32 weight);
extern s16 func_0026D628(Chooser *c);

s16 func_0025D030(Actor *actor, CAI *ai, Targets *t) {
    Chooser chooser;
    ActorInfo *info = &actor->def->info;
    s32 melee;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 mode;

    if (t->air.kind != 5) {
    melee = 0;
    a = 0;
    b = 0;
    c = 0;
    d = 0;
    mode = ai->modeIndex;
    if (t->ground.kind < 2) {
        if (mode == 0) {
            melee = info->meleeMask & 0x1F;
            a = info->maskA & 0x1F;
            b = info->maskB & 0x1F;
            c = info->maskC & 0x1F;
            d = info->maskD & 0x1F;
        } else if (mode == 1) {
            melee = info->meleeMask & 0x3E0;
            a = info->maskA & 0x3E0;
            b = info->maskB & 0x3E0;
            c = info->maskC & 0x3E0;
            d = info->maskD & 0x3E0;
        } else if (mode == 2) {
            melee = info->meleeMask & 0x7C00;
            a = info->maskA & 0x7C00;
            b = info->maskB & 0x7C00;
            c = info->maskC & 0x7C00;
            d = info->maskD & 0x7C00;
        }
    }
    func_0026D5DC(&chooser);
    if (t->air.kind != 7) {
        if (melee && t->air.visible && t->air.dist <= info->meleeRange) {
            func_0026D5E4(&chooser, 0xE, 100);
        }
        if (a && t->air.visible && info->rangeA - 20.48f <= t->air.dist && t->air.dist <= info->rangeA + 20.48f) {
            func_0026D5E4(&chooser, 0xF, 100);
        }
        if (b && t->air.visible && info->rangeB - 20.48f <= t->air.dist && t->air.dist <= info->rangeB + 20.48f) {
            func_0026D5E4(&chooser, 0x10, 100);
        }
    }
    if (c && ai->cooldown <= 0.0f && t->ground.dist <= info->rangeC) {
        func_0026D5E4(&chooser, 0x12, 100);
    }
    if (d && ai->cooldown <= 0.0f && t->ground.dist <= info->rangeD) {
        func_0026D5E4(&chooser, 0x11, 100);
    }
    if (chooser.count != 0) {
        goto pick;
    }
    }
    return 0;
pick:
    return func_0026D628(&chooser);
}
