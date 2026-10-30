/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SYSTEM_2/802661FC_2.CPP, func_802676EC_de (inlined here), with Turok 2's layouts. */

#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

/* A blast: radius, damage, then the push (strength, x, y, z). */
typedef struct Blast {
    u16 radius;
    u16 damage;
    s8 strength;
    s8 x;
    s8 y;
    s8 z;
} __attribute__((aligned(4))) Blast;

/* A shield record: its damage factor at 0x68. */
typedef struct {
    /* 0x00 */ u8 pad0[0x68];
    /* 0x68 */ f32 factor;
} Shield;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x996 - 0x10];
    /* 0x996 */ u16 weapon;
} BlastOwner;

typedef struct {
    /* 0x000 */ u8 kind;            /* 1: player, 2: object */
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x54 - 0x10];
    /* 0x054 */ f32 height;
    /* 0x058 */ u8 pad58[0xD4 - 0x58];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u8 ai[0x1A8 - 0x140];
    /* 0x1A8 */ BlastOwner *owner;
} BlastActor;

typedef struct {
    u8 data[0x18];
} HitInfo;

extern u8 D_8011AAF1;           /* multiplayer */
extern BlastActor *D_800F1CE0;  /* the carrier of the held shield */
extern s32 D_800F1CF4;          /* its slot, -1 for none */
extern Shield D_800F1CF8;       /* the held shield */
extern Shield D_800F1970;       /* the shield func_0021E374 finds */

s32 func_0023726C(BlastActor *victim, BlastActor *source);
void func_0020EF8C(Vec3f *out, f32 t, Vec3f *a, Vec3f *b);
f32 func_002671B4(void *actor);
f32 func_0021F268(BlastActor *actor);
s32 func_0021E374(BlastActor *actor, Vec3f pos, Shield *out);
f32 func_002672C8(BlastActor *actor);
f32 func_0020F6BC(Vec3f *a, Vec3f *b);
void func_002374AC(BlastActor *victim, BlastActor *source, Blast blast, Vec3f *out, u32 flags);
void func_00245828(HitInfo *out, BlastActor *source, s32 damage, f32 radius, u32 flags, Shield *shield);
void func_002465C8(BlastActor *victim, void *ai, HitInfo *hit);
void func_002376F8(BlastActor *victim, BlastActor *source, s32 hit, Vec3f pos, Vec3f dir, s32 push, Shield *shield, u32 flags);

/* How strongly the blast reaches the victim: through its shield, and with
 * falloff over the radius plus its width from the nearest point of its body. */
static __inline__ f32 blastScale(BlastActor *victim, BlastActor *source, Vec3f position, f32 radius, s32 falloff, u32 flags, Shield **found) {
    s32 shielded;
    s32 scaled;
    Shield *shield;
    f32 scale;
    f32 bottom;
    f32 top;
    f32 reach;
    f32 distance;
    Vec3f body;

    shielded = 1;
    scale = 1.0f;
    scaled = 1;
    if (D_8011AAF1 != 0 && (flags & 0x400000)) {
        shielded = 0;
    }
    if (source->kind == 2 && D_800F1CE0 == victim && D_800F1CF4 != -1) {
        shield = &D_800F1CF8;
        falloff = 0;
    } else {
        shield = func_0021E374(victim, position, &D_800F1970) ? &D_800F1970 : NULL;
    }
    *found = shield;
    if (shielded && shield != NULL) {
        scale *= shield->factor;
    }
    if (falloff != 0) {
        body = victim->pos;
        bottom = victim->pos.y + victim->height;
        top = bottom + func_002671B4(victim);
        if (top < position.y) {
            body.y = top;
        } else if (position.y < bottom) {
            body.y = bottom;
        } else {
            body.y = position.y;
        }
        reach = radius + func_002672C8(victim);
        distance = func_0020F6BC(&position, &body);
        if (reach < distance) {
            return 0.0f;
        }
        if (scaled) {
            scale *= 1.0f - distance / reach;
        }
    }
    return scale;
}

/* Applies a blast from source to victim: a player's own close-range shot
 * (no weapon, or weapon 0x22) is moved most of the way back to the player;
 * then damage, push and the hit report, scaled by blastScale. */
void func_00237AFC(BlastActor *victim, BlastActor *source, s32 hit, Vec3f pos, Blast blast, s32 falloff, s32 push, u32 flags) {
    Vec3f dir;
    HitInfo info;
    BlastOwner *owner;
    Shield *shield;
    f32 mult;
    s32 close;
    f32 radius;
    f32 damage;

    if (func_0023726C(victim, source) == 0) {
        return;
    }
    hit = 0;
    if (source->kind == 1 && (source->flags & 0x300000)) {
        owner = source->owner;
        if (source == (BlastActor *) ((u8 *) owner + 0x288) && (owner->weapon < 2 || (s16) owner->weapon == 0x22)) {
            Vec3f from;
            Vec3f to;

            from = pos;
            to = owner->pos;
            func_0020EF8C(&pos, 0.75f, &from, &to);
            pos.y += func_002671B4(owner) * 0.5f;
            hit = 1;
        }
    }
    close = hit;
    radius = blast.radius * 0.04f;
    if (source->kind == 1) {
        mult = func_0021F268(source);
    } else {
        mult = 1.0f;
    }
    radius *= mult;
    damage = blast.damage;
    damage *= blastScale(victim, source, pos, radius, falloff, flags, &shield);
    if (damage != 0.0f) {
        func_002374AC(victim, source, blast, &dir, flags);
        func_00245828(&info, source, damage, radius, flags, shield);
        func_002465C8(victim, victim->ai, &info);
        func_002376F8(victim, source, close, pos, dir, push, shield, flags);
    }
}
