#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct ActorPos {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ void *region;
    /* 0x14 */ s32 unk14;
} ActorPos; /* size 0x18 */

typedef struct HitBuffer {
    u8 data[0xF0];
} HitBuffer;

extern HitBuffer *D_800F1ADC;     /* current hit buffer */
typedef struct CollisionInfo {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s8 flagB;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s8 flagA;
} CollisionInfo;

extern CollisionInfo D_800F1CC0;
extern u8 D_80107590[];

void func_00239F00(ActorPos *actor, Vec3 pos, void *info);
Vec3 func_0020DFE0(Vec3 *in, f32 amount);
void func_0020F040(Vec3 *v);
void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);
u8 *func_00227240(void *table, void *slot, s32 kind, Vec3 velocity, Vec3 pos, void *region, f32 arg);

/* func_002688C8's logic, inlined with the collision flags addressed directly. */
static __inline__ void tracePos(ActorPos *actor, Vec3 pos, Vec3 *outPos, void **outRegion, s32 flagA, s32 flagB) {
    ActorPos saved;
    Vec3 end;
    HitBuffer hits;
    HitBuffer *oldHits;
    s32 oldA;
    s32 oldB;
    void *region;

    saved = *actor;
    oldA = D_800F1CC0.flagA;
    oldB = D_800F1CC0.flagB;
    oldHits = D_800F1ADC;
    D_800F1CC0.flagA = flagA;
    D_800F1CC0.flagB = flagB;
    D_800F1ADC = &hits;
    func_00239F00(actor, pos, &D_800F1CC0);
    actor->pos.y = pos.y;
    end.x = actor->pos.x;
    end.y = pos.y;
    end.z = actor->pos.z;
    region = actor->region;
    D_800F1ADC = oldHits;
    *actor = saved;
    *outPos = end;
    *outRegion = region;
    D_800F1CC0.flagA = oldA;
    D_800F1CC0.flagB = oldB;
}

/* Spawns a particle of the given kind where an actor would end up at pos. */
void func_00268C48(ActorPos *actor, Vec3 pos, s32 kind, f32 arg) {
    Vec3 up;
    Vec3 dir;
    Vec3 velocity;
    Vec3 at;
    void *region;

    tracePos(actor, pos, &at, &region, 0, 0);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    dir = func_0020DFE0(&up, 0.25f);
    func_0020F040(&dir);
    func_0020EF60(&velocity, &dir, 409.59998f);
    func_00227240(D_80107590, NULL, kind, velocity, at, region, arg);
}
