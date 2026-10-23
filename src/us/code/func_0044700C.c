#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0xB8 - 0x14];
    /* 0xB8 */ u16 typeId;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x24 - 0xC];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x9C - 0x2C];
    /* 0x9C */ Vec3 pos;
    /* 0xA8 */ s32 unkA8;
} Spawn;

typedef struct {
    /* 0x000 */ u8 pad0[0x50C];
    /* 0x50C */ s32 count454;
    /* 0x510 */ s32 count455;
    /* 0x514 */ s32 count456;
} Counters;

extern Counters D_80130200;

void func_0044700C(Actor *actor, Spawn *spawn) {
    Counters *counters;

    spawn->unk28 = 0;
    spawn->unk24 = 0;
    spawn->unk4 = spawn->unk8;
    actor->pos = spawn->pos;
    actor->unk10 = spawn->unkA8;
    counters = &D_80130200;
    switch (actor->typeId) {
        case 0x454:
            counters->count454++;
            break;
        case 0x455:
            counters->count455++;
            break;
        case 0x456:
            counters->count456++;
            break;
    }
}
