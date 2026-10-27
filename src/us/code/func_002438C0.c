/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_2.CPP, func_80214624_de, with Turok 2's layouts. */
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
} Location;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
} PlanInfo;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 active;
    /* 0x08 */ u8 pad8[0x30 - 8];
    /* 0x30 */ PlanInfo *info;
    /* 0x34 */ u8 pad34[0x68 - 0x34];
    /* 0x68 */ Actor *ignore;
    /* 0x6C */ u8 pad6C[0x80 - 0x6C];
    /* 0x80 */ Actor *approachFrom;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ u8 pad88[4];
    /* 0x8C */ f32 rotY;
    /* 0x90 */ f32 height;
    /* 0x94 */ s8 location;
    /* 0x95 */ u8 pad95[0xAC - 0x95];
    /* 0xAC */ Vec3f pos;
    /* 0xB8 */ s32 region;
} Plan;

extern s32 D_8011001C;

Location *func_0028479C(s8 *location);
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a + b */
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
void func_0020F040(Vec3f *v);                          /* normalise */
f32 func_0020EAA0(Vec3f *v);                           /* heading */
f32 func_002672C8(Actor *a);                           /* radius */
void func_002688C8(Actor *a, Vec3f pos, Vec3f *outPos, s32 *outRegion, s32 x, s32 y);
s32 func_00239C28(Actor *a, s32 b, Vec3f pos, s32 region, s32 d, Vec3f *out);
s32 func_002399F8(Actor *a, Vec3f pos, s32 region, s32 d, Vec3f *hit);
s32 func_0026BB9C(s32 region, f32 x, f32 z);
f32 func_00246310(Actor *a, Vec3f point);

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

s32 func_002438C0(Actor *self, Plan *plan, Actor *target) {
    Vec3f pos;
    Vec3f hit;
    s32 region;
    s32 mode;
    OwnerPlayer *player;
    Location *loc;

    if (plan->active == 0) {
        mode = 6;
    } else if (target == NULL) {
        mode = 3;
        if (plan->location != 0) {
            mode = 4;
        }
    } else {
        mode = 2;
        if (target != plan->ignore) {
            if (*target->def == 5) {
                mode = 5;
            } else if (target->typeId == 0x64F) {
                mode = 7;
            } else {
                mode = 0;
                if (D_8011001C == 0) {
                    if ((!(target->flags & 0x300000)
                         || (player = target->owner,
                             (!(player->unkAAC > 0.0f) || (mode = 1, self->typeId == 0xCA && player->weapon == 0x14))
                             && (player->unkB38 != self || (mode = 1, player->unkB2C != 2))))
                        && (!(self->flags27C & 2) || (mode = 1, self->typeId == 0xCA))) {
                        mode = 0;
                    }
                }
            }
        }
    }
    switch (mode) {
    case 6:
        pos = self->pos;
        region = self->region;
        break;
    case 4:
        loc = func_0028479C(&plan->location);
        pos = loc->pos;
        region = loc->region;
        break;
    case 3:
        pos = plan->pos;
        region = plan->region;
        break;
    case 7:
        Approach(self, target, plan->approachFrom, &pos, &region);
        break;
    case 1:
        region = func_00239C28(self, plan->unk84, target->pos, target->region, plan->info->unkC, &pos);
        if (region == 0) {
            do {
    default:
                pos = target->pos;
                region = target->region;
            } while (0);
        }
        break;
    }
    if (func_002399F8(self, pos, region, plan->info->unkC, &hit) != 0) {
        if (func_0026BB9C(self->region, hit.x, hit.z) != 0) {
            plan->height = hit.y;
        } else {
            plan->height = hit.y + 51.199997f;
        }
        func_0020EF2C(&hit, &hit, &self->pos);
        plan->rotY = func_0020EAA0(&hit) + 3.1415927f;
        return 1;
    }
    if (mode == 1) {
        plan->height = pos.y;
        plan->rotY = self->rotY + func_00246310(self, pos);
        return 1;
    }
    return 0;
}
