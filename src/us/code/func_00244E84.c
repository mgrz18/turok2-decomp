#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

typedef struct Actor {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0x40];
    /* 0x50 */ f32 yaw;
    /* 0x54 */ u8 pad54[0x80];
    /* 0xD4 */ u32 flags;
} Actor;

extern Actor *D_800F1CE0;       /* the carrier of the held shield */

f32 func_00298470(f32);         /* sqrtf */
f32 func_002982F0(f32);         /* sinf */
f32 func_002974C0(f32);         /* cosf */
f32 func_0021153C(f32);         /* acosf */
f32 func_002672C8(Actor *a);    /* radius */
s32 func_0026961C(Actor *self, Vec3f pos, s32 *out); /* blocked at pos */
void func_00210EF0(f32 *angle); /* wrap */

/* func_00246310: the angle from the actor's heading to a point. */
static __inline__ f32 angleTo(Actor *actor, Vec3f point) {
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
        s = func_002982F0(actor->yaw);
    } while (0);
    c = -func_002974C0(actor->yaw);
    dot = (dx * -s + dz * c) / dist;
    dot = ((((dot) > (1.0f) ? (1.0f) : (dot))) < ((-1.0f)) ? ((-1.0f)) : (((dot) > (1.0f) ? (1.0f) : (dot))));
    angle = func_0021153C(dot);
    if (!(dx * c + dz * s > 0.0f)) {
        angle = -angle;
    }
    return angle;
}

/* The heading for self toward pos: straight at it, or when pos is blocked,
 * the first free direction 30, 60, ... 150 degrees to either side (the side
 * tried first alternating between actors), turned 15 degrees further. */
f32 func_00244E84(Actor *self, Vec3f pos, Actor *target, f32 scale) {
    Vec3f probe;
    s32 out;
    f32 heading;
    f32 angle;
    f32 step;
    f32 extra;
    s32 i;
    s32 side;
    s32 first;

    angle = angleTo(self, pos);
    if ((self->flags & 0x200) && func_0026961C(self, pos, &out)) {
        if (target != NULL && D_800F1CE0 == target) {
            return angle;
        }
        probe.y = self->pos.y;
        first = ((u32) self % 0x288) & 1;
        for (i = 1; i < 6; i++) {
            step = i * 0.52359885f;
            for (side = 0; side < 2; side++) {
                heading = angle;
                if (side != first) {
                    heading = angle + step;
                    extra = 0.26179942f;
                } else {
                    heading = angle - step;
                    extra = -0.26179942f;
                }
                probe.x = self->pos.x + func_002982F0(self->yaw + 3.1415927f + heading) * (scale * func_002672C8(self));
                probe.z = self->pos.z + func_002974C0(self->yaw + 3.1415927f + heading) * (scale * func_002672C8(self));
                if (!func_0026961C(self, probe, NULL)) {
                    heading += extra;
                    func_00210EF0(&heading);
                    return heading;
                }
            }
        }
    }
    return angle;
}
