typedef signed int s32;
typedef unsigned short u16;
typedef float f32;

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ char pad0[0x4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ s32 region;
    /* 0x14 */ char pad14[0xA4];
    /* 0xB8 */ u16 type;
} Actor40E00C;

typedef struct {
    /* 0x00 */ char pad0[0x4];
    /* 0x04 */ s32 cur;
    /* 0x08 */ s32 start;
    /* 0x0C */ char padC[0x18];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ char pad2C[0x70];
    /* 0x9C */ Vec3f pos;
    /* 0xA8 */ s32 region;
} Spawn40E00C;

typedef struct {
    /* 0x000 */ char pad0[0x50C];
    /* 0x50C */ s32 count454;
    /* 0x510 */ s32 count455;
    /* 0x514 */ s32 count456;
} Stats80130200;

extern Stats80130200 D_80130200;

void func_0040E00C(Actor40E00C *actor, Spawn40E00C *spawn) {
    Stats80130200 *stats;

    spawn->unk28 = 0;
    spawn->unk24 = 0;
    spawn->cur = spawn->start;
    actor->pos = spawn->pos;
    actor->region = spawn->region;
    stats = &D_80130200;
    switch (actor->type) {
        case 0x454:
            stats->count454++;
            break;
        case 0x455:
            stats->count455++;
            break;
        case 0x456:
            stats->count456++;
            break;
    }
}
