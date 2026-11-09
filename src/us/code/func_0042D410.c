#include "common.h"
#include "structs/list.h"
#include "structs/swoosh.h"
#include "protos/list.h"

extern void NodePool_Init(void *pool, void *items, s32 size, s32 count);
extern void func_0026D230(void *list);
extern void DList_AddTail(void *list, void *node);
extern void SwooshSystem_Update(CSwooshSystem *world);

/* Builds the swoosh/effect system (structs/swoosh.h): the swoosh and timer pools, the free
 * swoosh points, the decal lists, an empty ribbon vertex pool. */
void func_0042D410(CSwooshSystem *world) {
    s32 i;

    NodePool_Init(&world->m_SwooshPool, world->m_Swooshes, sizeof(Swoosh), 32);
    NodePool_Init(&world->m_TimerPool, world->m_Timers, sizeof(SwooshTimer), 3);
    func_0026D230(&world->m_FreeNodes);
    for (i = 0; i < 96; i++) {
        DList_AddTail(&world->m_FreeNodes, &world->m_Nodes[i]);
    }
    CList__Construct(&world->m_FreeDecals, 0, sizeof(void *));
    for (i = 0; i < 10; i++) {
        CList__Construct(&world->m_DecalLists[i], 0, sizeof(void *));
    }
    for (i = 0; i < 100; i++) {
        CList__AddTail(&world->m_FreeDecals, &world->m_Decals[i]);
    }
    world->m_RibbonVtxLeft = 0;
    SwooshSystem_Update(world);
}
