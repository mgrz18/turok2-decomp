#include "common.h"

typedef struct IntrusiveList {
    /* 0x00 */ u8 pad0[0x14];
} IntrusiveList;

typedef struct World {
    /* 0x0000 */ u8 pad0[0x2580];
    /* 0x2580 */ s32 unk2580;
    /* 0x2584 */ u8 pad2584[4];
    /* 0x2588 */ u8 nodes[96][0xB8];
    /* 0x6A88 */ u8 nodeList[0xC];
    /* 0x6A94 */ u8 pool0Items[32][0x54];
    /* 0x7514 */ u8 pool0[0x18];
    /* 0x752C */ u8 pool1Items[3][0x1C];
    /* 0x7580 */ u8 pool1[0x18];
    /* 0x7598 */ u8 slots[100][0x50];
    /* 0x94D8 */ IntrusiveList lists[10];
    /* 0x95A0 */ IntrusiveList freeSlots;
} World;

extern void func_0026D330(void *pool, void *items, s32 size, s32 count);
extern void func_0026D230(void *list);
extern void func_0026D288(void *list, void *node);
extern void CList__Construct(IntrusiveList *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddTail(IntrusiveList *list, void *node);
extern void func_00235F48(World *world);

void func_0042D410(World *world) {
    s32 i;

    func_0026D330(world->pool0, world->pool0Items, 0x54, 32);
    func_0026D330(world->pool1, world->pool1Items, 0x1C, 3);
    func_0026D230(world->nodeList);
    for (i = 0; i < 96; i++) {
        func_0026D288(world->nodeList, world->nodes[i]);
    }
    CList__Construct(&world->freeSlots, 0, 4);
    for (i = 0; i < 10; i++) {
        CList__Construct(&world->lists[i], 0, 4);
    }
    for (i = 0; i < 100; i++) {
        CList__AddTail(&world->freeSlots, world->slots[i]);
    }
    world->unk2580 = 0;
    func_00235F48(world);
}
