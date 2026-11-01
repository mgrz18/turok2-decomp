/* WRONG CUT: REAL func_0042CF90 size 0x110 (splat cut it into func_0042CF90 + func_0042CFC0 + func_0042D040). */
#include "common.h"

/* A scene actor (0x1B0 bytes). */
typedef struct SceneActor {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ struct SceneActor *prev;
    /* 0x1AC */ struct SceneActor *next;
} SceneActor;

typedef struct ScenePool {
    /* 0x0000 */ SceneActor actors[32];
    /* 0x3600 */ SceneActor *free;
    /* 0x3604 */ SceneActor *head;
    /* 0x3608 */ s32 count;
} ScenePool;

/* The two default actor definitions. */
typedef struct ActorDef {
    /* 0x00 */ s32 type;
    /* 0x04 */ s32 flags;
    /* 0x08 */ u8 pad8[0xA];
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
} ActorDef;

extern ActorDef D_800F1360;
extern ActorDef D_800F12C0;

extern void func_0027BD10(ActorDef *def);
extern void func_00227664(SceneActor *actor);

/* Links the 32 actors of the pool into its free list and sets up the two default definitions. */
void func_0042CF90(ScenePool *pool) {
    s32 i;

    pool->free = pool->actors;
    pool->count = 0;
    pool->head = NULL;
    for (i = 0; i < 31; i++) {
        pool->actors[i].next = &pool->actors[i + 1];
        pool->actors[i].prev = &pool->actors[i - 1];
    }
    pool->actors[0].prev = NULL;
    pool->actors[31].next = NULL;

    func_0027BD10(&D_800F1360);
    D_800F1360.type = 8;
    D_800F1360.unk14 = 1;
    D_800F1360.unk12 = 0;
    D_800F1360.flags |= 4;

    func_0027BD10(&D_800F12C0);
    D_800F12C0.type = 8;
    D_800F12C0.unk14 = 0;
    D_800F12C0.unk12 = 0;
    D_800F12C0.flags |= 4;
    D_800F12C0.unk1C = D_800F12C0.unk18 = 122.88f;

    for (i = 0; i < 32; i++) {
        func_00227664(&pool->actors[i]);
    }
}
