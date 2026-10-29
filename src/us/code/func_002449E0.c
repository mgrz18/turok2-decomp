/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00215410.c, func_80215410_de, with Turok 2's layouts. */
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
    /* 0x054 */ u8 pad54[0xB8 - 0x54];
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
    /* 0x10 */ f32 rotY;
    /* 0x14 */ u16 flags;
} Location;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 active;
    /* 0x08 */ u8 pad8[0x34 - 8];
    /* 0x34 */ s8 state;
    /* 0x35 */ u8 pad35[0x68 - 0x35];
    /* 0x68 */ Actor *ignore;
    /* 0x6C */ u8 pad6C[0x78 - 0x6C];
    /* 0x78 */ Actor *forced;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ Actor *target;
    /* 0x84 */ u8 pad84[0x88 - 0x84];
    /* 0x88 */ Actor *altTarget;
    /* 0x8C */ u8 pad8C[0x94 - 0x8C];
    /* 0x94 */ s8 location;
    /* 0x95 */ u8 pad95[0x98 - 0x95];
    /* 0x98 */ f32 homeRotY;
    /* 0x9C */ u8 pad9C[0xAC - 0x9C];
    /* 0xAC */ Vec3f pos;
    /* 0xB8 */ u8 padB8[0xCA - 0xB8];
    /* 0xCA */ s8 team;
} Plan;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ Actor *target;
    /* 0x08 */ f32 angle;
    /* 0x0C */ Vec3f pos;
    /* 0x18 */ Vec3f delta;
    /* 0x24 */ f32 dist;
    /* 0x28 */ Vec3f flatPos;
    /* 0x34 */ Vec3f flatDelta;
    /* 0x40 */ f32 flatDist;
} TrackResult;

extern s32 D_800F7F78;
extern s32 D_8011001C;

Actor *func_00243C90(Actor *self, Plan *plan, s32 arg2, s32 arg3);
Location *func_0028479C(s8 *location);
f32 func_00246310(Actor *a, Vec3f point);              /* angle to a point */
void func_00210EF0(f32 *angle);                        /* wrap an angle */
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
f32 func_00298470(f32 x);                              /* sqrt */

/* What the plan aims at (the same classes as func_002438C0). */
static __inline__ s32 Kind(Actor *self, Plan *plan, Actor *target) {
    OwnerPlayer *player;

    if (plan->active == 0) {
        return 6;
    }
    if (target == NULL) {
        if (plan->location != 0) {
            return 4;
        }
        return 3;
    }
    if (target == plan->ignore) {
        return 2;
    }
    if (*target->def == 5) {
        return 5;
    }
    if (target->typeId == 0x64F) {
        return 7;
    }
    if (D_8011001C == 0) {
        if (target->flags & 0x300000) {
            player = target->owner;
            if (player->unkAAC > 0.0f && (self->typeId != 0xCA || player->weapon != 0x14)) {
                return 1;
            }
            if (player->unkB38 == self && player->unkB2C == 2) {
                return 1;
            }
        }
        if ((self->flags27C & 2) && self->typeId != 0xCA) {
            return 1;
        }
    }
    return 0;
}

/* Picks the plan's target and fills `out` with its kind, the turn to face
 * it, its position, offset and distance, in 3D and flattened. */
void func_002449E0(Actor *self, Plan *plan, s32 unused, TrackResult *out) {
    Vec3f pos;
    Vec3f delta;
    f32 angle;
    s32 kind;
    Actor *target;
    Location *loc;

    if (plan->team == D_800F7F78 && (!(plan->flags & 0x40000000) || plan->forced != NULL)) {
        plan->target = func_00243C90(self, plan, 1, 0);
    }
    if (*self->def == 1 && plan->state == 0xB && Kind(self, plan, plan->altTarget) == 4) {
        target = plan->altTarget;
        kind = 4;
    } else {
        target = plan->target;
        kind = Kind(self, plan, target);
    }
    if (target != NULL) {
        pos = target->pos;
        angle = func_00246310(self, pos);
    } else {
        switch (kind) {
        case 4:
            loc = func_0028479C(&plan->location);
            pos = loc->pos;
            if (loc->flags & 1) {
                angle = loc->rotY - self->rotY;
            } else {
                angle = func_00246310(self, pos);
            }
            break;
        case 3:
            pos = plan->pos;
            angle = plan->homeRotY - self->rotY;
            break;
        case 6:
            pos = self->pos;
            angle = 0.0f;
            break;
        }
    }
    func_00210EF0(&angle);
    func_0020EF2C(&delta, &pos, &self->pos);
    out->kind = kind;
    out->target = target;
    out->angle = angle;
    out->pos = pos;
    out->delta = delta;
    out->dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    delta.y = 0.0f;
    pos.y = 0;
    out->flatPos = pos;
    out->flatDelta = delta;
    out->flatDist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}
