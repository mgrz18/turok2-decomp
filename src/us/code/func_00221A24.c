#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/collision.h"
#include "structs/level_actor.h"

typedef struct List {
    s32 pad0;
    s32 count;                  /* 0x04 */
    u8 items[1];                /* 0x08 */
} List;

extern s32 D_8011AAE0;          /* visible layers */
extern s32 D_800ACADC;
extern CBox D_800F5A10;       /* visible bounds */

void *func_002017D4(void *table, s32 index);
void func_00214E88(CStaticObject *inst);
s32 func_00267090(CStaticObject *inst);
f32 func_002671B4(CLevelActor *actor);
void func_002254C0(CWorld *world, CVector3 *pos, void *out);
void func_002156FC(CLevelActor *actor);
void func_002279A4(void *arg0, CWorld *world);

/* Collects the instances and actors of the loaded sections inside the visible bounds. */
void func_00221A24(CWorld *world) {
    CVector3 pos;
    s32 regionCount;
    void *data;
    CWorldSector *regions;
    List *list;
    CStaticObject *inst;
    CStaticObject *insts;
    CLevelActor *actors;
    CLevelActor *actor;
    s32 layers;
    s32 i;
    s32 j;
    s32 instCount;
    s32 actorCount;

    i = 0;
    regionCount = world->m_nSectors;
    layers = D_8011AAE0;
    regions = world->m_Sectors;
    for (; i < regionCount; i++) {
        data = *regions[i].m_hData;
        j = 0;
        list = func_002017D4(data, 0);
        instCount = list->count;
        insts = (CStaticObject *)list->items;
        for (; j < instCount; j++) {
            inst = &insts[j];
            if (!(inst->header.m_pIntelligence->m_dwFlags & layers)) {
                continue;
            }
            if (D_800F5A10.m_vMax.x > inst->m_Bounds.m_vMin.x && D_800F5A10.m_vMin.x < inst->m_Bounds.m_vMax.x
                && D_800F5A10.m_vMax.z > inst->m_Bounds.m_vMin.z && D_800F5A10.m_vMin.z < inst->m_Bounds.m_vMax.z
                && D_800F5A10.m_vMax.y > inst->m_Bounds.m_vMin.y && D_800F5A10.m_vMin.y < inst->m_Bounds.m_vMax.y) {
                func_00214E88(inst);
                if (func_00267090(inst) && world->m_nDrawActors != 0x200) {
                    world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)inst;
                }
            }
        }
        list = func_002017D4(data, 1);
        j = 0;
        actorCount = list->count;
        actors = (CLevelActor *)list->items;
        for (; j < actorCount; j++) {
            actor = &actors[j];
            if (!(actor->header.m_pIntelligence->m_dwFlags & layers)) {
                continue;
            }
            if (actor->m_wFlags & 0x41) {
                continue;
            }
            if (D_800ACADC != 0) {
                pos.x = actor->header.m_vPos.x;
                pos.y = actor->header.m_vPos.y + func_002671B4(actor) * 0.5f;
                pos.z = actor->header.m_vPos.z;
                func_002254C0(world, &pos, actor->m_RegionLinks);
            }
            if (D_800F5A10.m_vMax.x > actor->m_Bounds.m_vMin.x && D_800F5A10.m_vMin.x < actor->m_Bounds.m_vMax.x
                && D_800F5A10.m_vMax.z > actor->m_Bounds.m_vMin.z && D_800F5A10.m_vMin.z < actor->m_Bounds.m_vMax.z
                && D_800F5A10.m_vMax.y > actor->m_Bounds.m_vMin.y && D_800F5A10.m_vMin.y < actor->m_Bounds.m_vMax.y) {
                func_002156FC(actor);
                if (world->m_nDrawActors != 0x200) {
                    world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)actor;
                }
            }
        }
    }
    func_002279A4(world->m_ActorPool, world);
}
