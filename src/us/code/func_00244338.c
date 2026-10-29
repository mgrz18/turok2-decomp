/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_3.CPP, func_80214DD4_de, with Turok 2's layouts. */
#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

/* The player behind an object (owner 0x1A8 of a player-controlled one). */
typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xAAC - 0x998];
    /* 0xAAC */ f32 unkAAC;
    /* 0xAB0 */ u8 padAB0[0xB2C - 0xAB0];
    /* 0xB2C */ s32 unkB2C;
    /* 0xB30 */ u8 padB30[0xB38 - 0xB30];
    /* 0xB38 */ struct Actor *unkB38;
} OwnerPlayer;

typedef struct Actor {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ s32 region;
    /* 0x014 */ s32 *def;           /* first word: kind */
    /* 0x018 */ u8 pad18[0x50 - 0x18];
    /* 0x050 */ f32 rotY;
    /* 0x054 */ f32 eye;
    /* 0x058 */ u8 pad58[0xB8 - 0x58];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[0xD4 - 0xBA];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ OwnerPlayer *owner;
    /* 0x1AC */ u8 pad1AC[0x27C - 0x1AC];
    /* 0x27C */ u32 flags27C;
} Actor;

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s32 region;
} Location;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 active;
    /* 0x08 */ u8 pad8[0x68 - 8];
    /* 0x68 */ Actor *ignore;
    /* 0x6C */ Vec3f offset;
    /* 0x78 */ Actor *forced;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ Actor *approachFrom;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ Actor *target;
    /* 0x8C */ f32 rotY;
    /* 0x90 */ f32 height;
    /* 0x94 */ s8 location;
    /* 0x95 */ u8 pad95[0xAC - 0x95];
    /* 0xAC */ Vec3f pos;
    /* 0xB8 */ s32 region;
    /* 0xBC */ u8 padBC[0xCA - 0xBC];
    /* 0xCA */ s8 team;
} Plan;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ Actor *target;
    /* 0x08 */ f32 range;
    /* 0x0C */ Vec3f pos;
    /* 0x18 */ Vec3f delta;
    /* 0x24 */ f32 dist;
    /* 0x28 */ Vec3f flatPos;
    /* 0x34 */ Vec3f flatDelta;
    /* 0x40 */ f32 flatDist;
} TrackResult;

extern s32 D_800F7F78;
extern s32 D_8011001C;

s32 func_0025E428(Actor *self, Plan *plan);
s32 func_002438C0(Actor *self, Plan *plan, Actor *target);
Actor *func_00243C90(Actor *self, Plan *plan, s32 arg2, s32 arg3);
f32 func_00244E84(Actor *self, Vec3f pos, Actor *target, f32 scale);
f32 func_00246310(Actor *a, Vec3f point);              /* angle to a point */
Location *func_0028479C(s8 *location);
f32 func_002671B4(Actor *a);                           /* height */
f32 func_002672C8(Actor *a);                           /* radius */
f32 func_00268460(Actor *a);                           /* reach */
f32 func_00268A5C(Actor *a);                           /* ceiling */
void func_002688C8(Actor *a, Vec3f pos, Vec3f *outPos, s32 *outRegion, s32 x, s32 y);
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a + b */
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
void func_0020F040(Vec3f *v);                          /* normalise */
f32 func_0020EAA0(Vec3f *v);                           /* heading */
f32 func_002982F0(f32 x);                              /* sin */
f32 func_002974C0(f32 x);                              /* cos */
f32 func_00298470(f32 x);                              /* sqrt */

/* The point beside `target`, on the side away from `from`, where `self`
 * stands touching it. */
static __inline__ void Approach(Actor *self, Actor *target, Actor *from, Vec3f *outPos, s32 *outRegion) {
    Vec3f dir;
    Vec3f to;
    f32 dist;

    if (from != NULL) {
        func_0020EF2C(&dir, &target->pos, &from->pos);
        dir.y = 0.0f;
        func_0020F040(&dir);
    } else {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 0.0f;
    }
    dist = func_002672C8(target) + func_002672C8(self) + 51.199997f;
    func_0020EF60(&dir, &dir, dist);
    func_0020EEF8(&to, &target->pos, &dir);
    func_002688C8(target, to, outPos, outRegion, 0, 0);
}

/* Picks what the plan aims at (its target, itself, a location or home
 * point, or the actor) and fills `out` with the kind, target, range, goal
 * position and its offset from the actor, in 3D and flattened. */
void func_00244338(Actor *self, Plan *plan, s32 boost, TrackResult *out) {
    Vec3f pos;
    Vec3f probe;
    Vec3f away;
    Vec3f delta;
    s32 region;
    Actor *target;
    s32 kind;
    s32 mode;
    f32 range;
    f32 gap;
    f32 angle;
    f32 reach;
    OwnerPlayer *player;
    Location *loc;

    boost = 0; /* FAKEMATCH: the unused third parameter doubles as the boost flag (as in Rage Wars' func_80214DD4_de); a separate local gives the same code but swaps $s5/$s6 in the prologue. */
    if (*self->def == 1) {
        mode = func_0025E428(self, plan);
    } else {
        mode = -1;
    }
    if (plan->team == D_800F7F78) {
        if ((plan->flags & 0x20000000) && plan->forced == NULL) {
            if (func_002438C0(self, plan, plan->target) == 0) {
                plan->flags &= ~0x20000000;
            }
        } else {
            plan->target = func_00243C90(self, plan, 0, 1);
        }
    }
    target = plan->target;
    if (plan->active == 0) {
        kind = 6;
    } else if (target == NULL) {
        kind = 3;
        if (plan->location != 0) {
            kind = 4;
        }
    } else {
        kind = 2;
        if (target != plan->ignore) {
            if (*target->def == 5) {
                kind = 5;
            } else if (target->typeId == 0x64F) {
                kind = 7;
            } else {
                kind = 0;
                if (D_8011001C == 0) {
                    if ((!(target->flags & 0x300000)
                         || (player = target->owner,
                             (!(player->unkAAC > 0.0f) || (kind = 1, self->typeId == 0xCA && player->weapon == 0x14))
                             && (player->unkB38 != self || (kind = 1, player->unkB2C != 2))))
                        && (!(self->flags27C & 2) || (kind = 1, self->typeId == 0xCA))) {
                        kind = 0;
                    }
                }
            }
        }
    }
    if (target != NULL) {
        if (kind == 7) {
            Approach(self, target, plan->approachFrom, &pos, &region);
        } else {
            pos = target->pos;
            region = target->region;
            if (mode == 1) {
                pos.y += target->eye + func_002671B4(target) * 0.9f;
            }
        }
        gap = func_002672C8(target) + func_002672C8(self);
        switch (kind) {
        case 2:
            gap += 102.399994f;
            break;
        case 5:
        case 7:
            gap = 0.0f;
            break;
        }
        boost = 1;
        if (plan->location == 1) {
            plan->location = 2;
        } else if (plan->location == 3) {
            plan->location = 4;
        }
    } else {
        gap = func_002672C8(self);
        switch (kind) {
        case 4:
            loc = func_0028479C(&plan->location);
            pos = loc->pos;
            gap = 0.0f;
            region = loc->region;
            if (plan->location == 2 || plan->location == kind) {
                boost = 1;
            }
            break;
        case 3:
            pos = plan->pos;
            region = plan->region;
            boost = 1;
            break;
        case 6:
            pos = self->pos;
            region = self->region;
            if (mode == 2) {
                pos.y = func_00268A5C(self);
            }
            break;
        }
    }
    func_0020EEF8(&pos, &pos, &plan->offset);
    if (gap > 0.0f) {
        func_0020EF2C(&away, &self->pos, &pos);
        angle = func_0020EAA0(&away);
        pos.x -= gap * func_002982F0(angle);
        pos.z -= gap * func_002974C0(angle);
    }
    if (mode == 2) {
        pos.y = (pos.y < func_00268A5C(self)) ? pos.y : func_00268A5C(self);
    }
    if (boost) {
        reach = func_00268460(self);
        probe.x = self->pos.x + reach * func_002982F0(plan->rotY);
        probe.y = plan->height;
        probe.z = self->pos.z + reach * func_002974C0(plan->rotY);
        range = func_00244E84(self, probe, target, 1.5f);
        pos.y = plan->height;
    } else {
        range = func_00246310(self, pos);
    }
    func_0020EF2C(&delta, &pos, &self->pos);
    out->kind = kind;
    out->target = target;
    out->range = range;
    out->pos = pos;
    out->delta = delta;
    out->dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    delta.y = 0.0f;
    pos.y = 0.0f;
    out->flatPos = pos;
    out->flatDelta = delta;
    out->flatDist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}
