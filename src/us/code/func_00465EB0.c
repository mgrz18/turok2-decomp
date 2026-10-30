#include "common.h"

/* Reset the world's pool of run-time actors (all on the free list) and the two
 * pickup effect emitters. */

typedef struct PoolActor {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ struct PoolActor *prev;
    /* 0x1AC */ struct PoolActor *next;
} PoolActor; /* 0x1B0 bytes */

typedef struct ActorPool {
    /* 0x0000 */ PoolActor items[32];
    /* 0x3600 */ PoolActor *free;
    /* 0x3604 */ PoolActor *head; /* newest */
    /* 0x3608 */ PoolActor *tail; /* oldest */
} ActorPool;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u8 pad8[0xA];
    /* 0x12 */ u8 count;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ s32 active;
    /* 0x18 */ f32 width;
    /* 0x1C */ f32 height;
} Emitter;

extern Emitter D_800F1360;
extern Emitter D_800F12C0;

void func_0027BD10(Emitter *emitter);
void func_00227664(PoolActor *actor);

void func_00465EB0(ActorPool *pool) {
    s32 i;

    pool->free = pool->items;
    pool->tail = NULL;
    pool->head = NULL;
    for (i = 0; i < 31; i++) {
        pool->items[i].next = &pool->items[i + 1];
        pool->items[i].prev = &pool->items[i - 1];
    }
    pool->items[0].prev = NULL;
    pool->items[31].next = NULL;

    func_0027BD10(&D_800F1360);
    D_800F1360.kind = 8;
    D_800F1360.active = 1;
    D_800F1360.count = 0;
    D_800F1360.flags |= 4;
    func_0027BD10(&D_800F12C0);
    D_800F12C0.kind = 8;
    D_800F12C0.active = 0;
    D_800F12C0.count = 0;
    D_800F12C0.flags |= 4;
    D_800F12C0.width = 122.88f;
    D_800F12C0.height = 122.88f;

    for (i = 0; i < 32; i++) {
        func_00227664(&pool->items[i]);
    }
}
