/* NON-MATCHING: 8 of 265 words differ (W batch). */
/* NON-MATCHING: 8 of 265 words differ: case 0 actor-count loop takes v1/v0 instead of the ROM a0/v1 (register choice only). */
#include "common.h"

/* Boss animation event callback: by event, spawns a minion (fewer than 3
 * actors), hits the target's owner with a shock wave when close, throws the
 * live minions around the target, or knocks the target back. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s32 type;
} AnimEvent;

typedef struct {
    s16 a;
    s16 b;
    u8 c[4];
} __attribute__((aligned(4))) Part;

typedef struct Actor {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x18 - 0x10];
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0x50 - 0x24];
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0xD4 - 0x54];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u8 ai[0x1A8 - 0x140];
    /* 0x1A8 */ struct Player *owner;
    /* 0x1AC */ u8 pad1AC[0x28C - 0x1AC];
    /* 0x28C */ struct Actor *next;
} Actor;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0xA08 - 0x10];
    /* 0xA08 */ s16 state;
} Player;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Actor *actor;
    /* 0x08 */ u8 pad8[0x40 - 0x8];
    /* 0x40 */ f32 dist;
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x000 */ u8 pad0[0x630];
    /* 0x630 */ Actor *actors[7];
    /* 0x64C */ s32 count;
    /* 0x650 */ u8 pad650[0x658 - 0x650];
    /* 0x658 */ s32 coreCount;
    /* 0x65C */ s32 phase;
    /* 0x660 */ u8 pad660[0x670 - 0x660];
    /* 0x670 */ s32 alive;
} State;

extern State D_8012FB70;
extern Actor *D_8010FEB4;
extern u8 D_8010ABA0[];

extern void func_00246108(Actor *obj, void *ai, Targets *t, s32 f1, s32 f2);
extern void func_002688C8(void *obj, Vec3f in, Vec3f *out, s32 *region, s32 a, s32 b);
extern void *func_00228108(void *world, s32 model, s32 type, s32 intelligence, s32 region, Vec3f pos, f32 rotY,
                           Vec3f vel, Vec3f scale, s32 *owner);
extern void func_00236EA0(void *src, void *dst, s32 kind, s32 id, Vec3f pos, Part part);
extern f32 func_002119FC(f32 lo, f32 hi);
extern f32 func_002982F0(f32);
extern f32 func_002974C0(f32);
extern void func_00243414(Actor *obj, void *ai, s32 state);
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
extern void func_0020F040(Vec3f *v);
extern void func_0020EF60(Vec3f *out, Vec3f *a, f32 s);
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);

static __inline__ s32 countActors(void) {
    Actor *actor;
    s32 n;

    n = 0;
    for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
        n++;
    }
    return n;
}

/* func_0040BD60 */
static __inline__ f32 getTargetDist(Actor *obj, Actor **found) {
    Targets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, obj->ai, &t, 1, 0);
    if (t.b.actor != NULL && (t.b.actor->flags & 0x300000)) {
        dist = t.b.dist;
        *found = t.b.actor;
    }
    return dist;
}

void func_00409D4C(s32 arg0, void *self, s32 arg2, AnimEvent ev) {
    State *s = &D_8012FB70;
    Actor *obj = self;
    Actor *found;
    Player *owner;
    Actor *actor;
    f32 dist;
    s32 i;
    s32 region;
    Vec3f scale;
    f32 r;
    f32 angle;
    f32 dx;
    f32 dz;
    f32 x;
    f32 z;

    dist = getTargetDist(obj, &found);
    owner = NULL;
    if (found != NULL) {
        owner = found->owner;
    }
    switch (ev.type) {
    case 0:
        if (countActors() < 3 && (s->phase == 2 || 1228.8f < dist)) {
            func_002688C8(self, ev.pos, &ev.pos, &region, 0, 0);
            scale.x = 0.5f;
            scale.y = 0.5f;
            scale.z = 0.5f;
            func_00228108(D_8010ABA0, -1, 0x458, s->coreCount, region, ev.pos, 0.0f, obj->vel, scale, NULL);
        }
        break;
    case 1:
        if (found != NULL && owner != NULL && dist < 1228.8f && owner->state != 5) {
            Part part;

            part.a = 0xA00;
            part.b = 0x500;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            func_00236EA0(obj, obj, 3, -1, owner->pos, part);
        }
        break;
    case 2:
        if (found != NULL && owner != NULL) {
            for (i = 0; i < s->alive; i++) {
                actor = s->actors[i];
                r = func_002119FC(153.59999f, 256.0f);
                angle = found->yaw + 3.1415927f + func_002119FC(-0.5235988f, 0.5235988f);
                x = owner->pos.x;
                z = owner->pos.z;
                dx = r * func_002982F0(angle);
                dz = r * func_002974C0(angle);
                x += dx;
                z += dz;
                actor->pos.y = func_002119FC(-204.79999f, -51.199997f);
                actor->pos.x = x;
                actor->pos.z = z;
                func_00243414(actor, actor->ai, 1);
            }
        }
        break;
    case 3:
        if (found != NULL && owner != NULL) {
            Part part;

            part.a = 0xA00;
            part.b = 0xA00;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            func_00236EA0(obj, obj, 3, -1, owner->pos, part);
        }
        break;
    case 4:
        if (found != NULL && owner != NULL && dist < 1433.6f) {
            Part part;

            part.a = 0xA00;
            part.b = 0x500;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            func_00236EA0(obj, obj, 3, -1, owner->pos, part);
        }
        break;
    case 5:
        if (found != NULL) {
            Vec3f push;

            func_0020EF2C(&push, &obj->pos, &found->pos);
            func_0020F040(&push);
            func_0020EF60(&push, &push, dist);
            func_0020EEF8(&found->vel, &found->vel, &push);
        }
        break;
    }
}
