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
extern s8 D_800F1CC6[];           /* collision info D_800F1CC0 + 6 */

void func_00239F00(ActorPos *actor, Vec3 pos, void *info);

/* Moves an actor to pos with collision, without moving it: returns where it would end up and in which region. */
void func_002688C8(ActorPos *actor, Vec3 pos, Vec3 *outPos, void **outRegion, s32 flagA, s32 flagB) {
    ActorPos saved;
    Vec3 end;
    HitBuffer hits;
    HitBuffer *oldHits;
    s32 oldA;
    s32 oldB;
    void *region;
    s8 *flags;

    saved = *actor;
    oldHits = D_800F1ADC;
    D_800F1ADC = &hits;
    flags = D_800F1CC6;
    oldA = flags[0];
    oldB = flags[-2];
    flags[0] = flagA;
    flags[-2] = flagB;
    func_00239F00(actor, pos, flags - 6);
    D_800F1ADC = oldHits;
    actor->pos.y = pos.y;
    end.x = actor->pos.x;
    end.y = pos.y;
    end.z = actor->pos.z;
    region = actor->region;
    *actor = saved;
    *outPos = end;
    *outRegion = region;
    flags[0] = oldA;
    flags[-2] = oldB;
}
