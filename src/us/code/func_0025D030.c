#include "common.h"
#include "structs/intelligence.h"

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
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ CIntelligence *def;
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
    CEnemyIntelligence *info = &actor->def->m_Enemy;
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
            melee = info->m_wCombatAnimFlags & 0x1F;
            a = info->m_wLeapAnimFlags & 0x1F;
            b = info->m_wDartAnimFlags & 0x1F;
            c = info->m_wProjectileAnimFlags & 0x1F;
            d = info->m_wWeaponAnimFlags & 0x1F;
        } else if (mode == 1) {
            melee = info->m_wCombatAnimFlags & 0x3E0;
            a = info->m_wLeapAnimFlags & 0x3E0;
            b = info->m_wDartAnimFlags & 0x3E0;
            c = info->m_wProjectileAnimFlags & 0x3E0;
            d = info->m_wWeaponAnimFlags & 0x3E0;
        } else if (mode == 2) {
            melee = info->m_wCombatAnimFlags & 0x7C00;
            a = info->m_wLeapAnimFlags & 0x7C00;
            b = info->m_wDartAnimFlags & 0x7C00;
            c = info->m_wProjectileAnimFlags & 0x7C00;
            d = info->m_wWeaponAnimFlags & 0x7C00;
        }
    }
    func_0026D5DC(&chooser);
    if (t->air.kind != 7) {
        if (melee && t->air.visible && t->air.dist <= info->m_AttackCombatRadius) {
            func_0026D5E4(&chooser, 0xE, 100);
        }
        if (a && t->air.visible && info->m_AttackLeapRadius - 20.48f <= t->air.dist && t->air.dist <= info->m_AttackLeapRadius + 20.48f) {
            func_0026D5E4(&chooser, 0xF, 100);
        }
        if (b && t->air.visible && info->m_AttackDartRadius - 20.48f <= t->air.dist && t->air.dist <= info->m_AttackDartRadius + 20.48f) {
            func_0026D5E4(&chooser, 0x10, 100);
        }
    }
    if (c && ai->cooldown <= 0.0f && t->ground.dist <= info->m_AttackProjectileRadius) {
        func_0026D5E4(&chooser, 0x12, 100);
    }
    if (d && ai->cooldown <= 0.0f && t->ground.dist <= info->m_AttackWeaponRadius) {
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
