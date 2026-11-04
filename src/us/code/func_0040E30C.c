#include "common.h"
#include "structs/boss.h"

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

extern s32 func_00243414(Actor *actor, void *ai, s32 state);

void func_0040E30C(Actor *actor, Spawn *spawn) {
    BlindOneState *counters = &D_80130200;
    Actor *next;

    if (((Actor *)counters->obj)->state != 1) {
        func_00243414((Actor *)counters->obj, ((Actor *)counters->obj)->ai, 8);
    }
    spawn->hits++;
    switch (actor->type) {
        case 0x454:
            counters->alive1--;
            break;
        case 0x455:
            counters->alive2--;
            break;
        case 0x456:
            counters->alive3--;
            next = (Actor *)counters->actors3[(spawn->index + 1) % counters->count3];
            if (next->unk24C == 0) {
                func_00243414(next, next->ai, 0);
            }
            break;
    }
}
