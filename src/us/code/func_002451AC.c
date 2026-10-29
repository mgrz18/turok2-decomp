/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00215B90.c, func_00215B90, with Turok 2's layouts. */
#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA64 - 0x998];
    /* 0xA64 */ u32 flagsA64;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ s32 *def;           /* first word: kind */
    /* 0x018 */ u8 pad18[0x50 - 0x18];
    /* 0x050 */ f32 rotY;
    /* 0x054 */ f32 eye;
    /* 0x058 */ u8 pad58[0xD4 - 0x58];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x174 - 0xD8];
    /* 0x174 */ s8 state;
    /* 0x175 */ u8 pad175[0x1A8 - 0x175];
    /* 0x1A8 */ Player *owner;
    /* 0x1AC */ u8 pad1AC[0x204 - 0x1AC];
    /* 0x204 */ s16 anim;
} Actor;

s32 func_0024E700(Player *player, s32 weapon);   /* ammo for a weapon */
f32 func_002671B4(Actor *a);                     /* height */
f32 func_0021153C(f32 x);                        /* acos */
f32 func_002982F0(f32 x);                        /* sin */
f32 func_002974C0(f32 x);                        /* cos */
f32 func_00298470(f32 x);                        /* sqrt */
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
void func_0020F040(Vec3f *v);                          /* normalise */
void func_0021F00C(Vec3f *out, Actor *a, Vec3f v);    /* v in a's frame */

/* func_00246310: the signed angle from the actor's heading to a point. */
static __inline__ f32 AngleTo(Actor *actor, Vec3f point) {
    f32 dx;
    f32 dz;
    f32 dist;
    f32 s;
    f32 c;
    f32 dot;
    f32 angle;

    dx = point.x - actor->pos.x;
    dz = point.z - actor->pos.z;
    dist = func_00298470(dx * dx + dz * dz);
    if (dist == 0.0f) {
        return 0.0f;
    }
    do {
        s = func_002982F0(actor->rotY);
    } while (0);
    c = -func_002974C0(actor->rotY);
    dot = (dx * -s + dz * c) / dist;
    dot = ((((dot) > (1.0f) ? (1.0f) : (dot))) < ((-1.0f)) ? ((-1.0f)) : (((dot) > (1.0f) ? (1.0f) : (dot))));
    angle = func_0021153C(dot);
    if (!(dx * c + dz * s > 0.0f)) {
        angle = -angle;
    }
    return angle;
}

/* Whether `self` sees `target` (a player holding a ready weapon, or a
 * creature in an attack pose) aiming at it: 0 if not, else the side (-1/1). */
s32 func_002451AC(Actor *self, s32 unused, Actor *target, f32 cone) {
    Vec3f dir;
    Vec3f facing;
    Vec3f fwd;
    Vec3f from;
    Vec3f to;
    f32 lo;
    f32 hi;
    f32 y;
    f32 angle;
    Player *player;

    if (target == NULL) {
        return 0;
    }
    if (target->flags & 0x300000) {
        player = target->owner;
        if (func_0024E700(player, player->weapon) <= 0) {
            return 0;
        }
        if (!(player->flagsA64 & 0x2000)) {
            return 0;
        }
        goto body;
    }
    switch (*target->def) {
    case 1:
        if (target->anim < 0x10CC) {
            return 0;
        }
        goto body;
    case 4:
        if (target->state != 0x3F) {
            return 0;
        }
        goto body;
    default:
        return 0;
    }
body:
    lo = self->pos.y + self->eye;
    hi = lo + func_002671B4(self);
    to.x = target->pos.x;
    y = target->pos.y + target->eye + func_002671B4(target) * 0.9f;
    to.y = y;
    to.z = target->pos.z;
    from.x = self->pos.x;
    from.z = self->pos.z;
    if (hi < y) {
        from.y = hi;
    } else if (y < lo) {
        from.y = lo;
    } else {
        from.y = y;
    }
    angle = (AngleTo(self, to) < 0.0f) ? -AngleTo(self, to) : AngleTo(self, to);
    if (1.2217306f < angle && angle < 1.9198624f) {
        return 0;
    }
    func_0020EF2C(&dir, &from, &to);
    func_0020F040(&dir);
    fwd.x = 0;
    fwd.y = 0;
    fwd.z = 1.0f;
    func_0021F00C(&facing, target, fwd);
    if (func_0021153C(dir.x * facing.x + dir.y * facing.y + dir.z * facing.z) < cone * 0.5f) {
        if (dir.x * -facing.z + dir.z * facing.x < 0.0f) {
            return -1;
        }
        return 1;
    }
    return 0;
}
