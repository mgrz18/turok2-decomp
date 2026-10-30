#include "common.h"

typedef struct Actor Actor;

struct Actor {
    /* 0x000 */ u8 pad0[0xB8];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ u8 padBA[0x140 - 0xBA];
    /* 0x140 */ u8 ai[0x34];
    /* 0x174 */ s8 state;
    /* 0x175 */ u8 pad175[0x24C - 0x175];
    /* 0x24C */ s32 unk24C;
};

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ s32 hits;
    /* 0x110 */ s32 index;
} Spawn;

typedef struct {
    /* 0x000 */ Actor *leader;
    /* 0x004 */ u8 pad4[0x2C - 0x4];
    /* 0x02C */ s32 memberCount;
    /* 0x030 */ Actor *members[(0x50C - 0x30) / 4];
    /* 0x50C */ s32 count454;
    /* 0x510 */ s32 count455;
    /* 0x514 */ s32 count456;
} Counters;

extern Counters D_80130200;

extern s32 func_00243414(Actor *actor, void *ai, s32 state);

void func_0040E30C(Actor *actor, Spawn *spawn) {
    Counters *counters = &D_80130200;
    Actor *next;

    if (counters->leader->state != 1) {
        func_00243414(counters->leader, counters->leader->ai, 8);
    }
    spawn->hits++;
    switch (actor->type) {
        case 0x454:
            counters->count454--;
            break;
        case 0x455:
            counters->count455--;
            break;
        case 0x456:
            counters->count456--;
            next = counters->members[(spawn->index + 1) % counters->memberCount];
            if (next->unk24C == 0) {
                func_00243414(next, next->ai, 0);
            }
            break;
    }
}
