#include "common.h"
#include "structs/list.h"
#include "protos/list.h"

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
    /* 0x94D8 */ CList lists[10];
    /* 0x95A0 */ CList freeSlots;
} World;

extern void NodePool_Init(void *pool, void *items, s32 size, s32 count);
extern void func_0026D230(void *list);
extern void DList_AddTail(void *list, void *node);
extern void SwooshSystem_Update(World *world);

void func_0042D410(World *world) {
    s32 i;

    NodePool_Init(world->pool0, world->pool0Items, 0x54, 32);
    NodePool_Init(world->pool1, world->pool1Items, 0x1C, 3);
    func_0026D230(world->nodeList);
    for (i = 0; i < 96; i++) {
        DList_AddTail(world->nodeList, world->nodes[i]);
    }
    CList__Construct(&world->freeSlots, 0, sizeof(void *));
    for (i = 0; i < 10; i++) {
        CList__Construct(&world->lists[i], 0, sizeof(void *));
    }
    for (i = 0; i < 100; i++) {
        CList__AddTail(&world->freeSlots, world->slots[i]);
    }
    world->unk2580 = 0;
    SwooshSystem_Update(world);
}
