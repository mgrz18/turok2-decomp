#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/collision.h"
#include "structs/level_actor.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "protos/iset.h"
#include "protos/scene.h"

typedef struct List {
    s32 pad0;
    s32 count;                  /* 0x04 */
    u8 items[1];                /* 0x08 */
} List;

void CStaticObject__TouchLitVertices(CStaticObject *inst);
s32 GameObject_IsSolid(CStaticObject *inst);
f32 GameObject_GetCollisionHeight(CLevelActor *actor);
void CLevelActor__Update(CLevelActor *actor);
void CLevelActorPool__CollectVisible(CLevelActorPool *arg0, CScene *world);

/* Collects the instances and actors of the loaded sections inside the visible bounds. */
void CScene__CollectSectorObjects(CScene *world) {
    CVector3 pos;
    s32 regionCount;
    void *data;
    CSceneSector *regions;
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
    layers = gEngineApp_m_Settings.difficulty;
    regions = world->m_Sectors;
    for (; i < regionCount; i++) {
        data = *regions[i].m_hData;
        j = 0;
        list = CISet__GetBlock(data, 0);
        instCount = list->count;
        insts = (CStaticObject *)list->items;
        for (; j < instCount; j++) {
            inst = &insts[j];
            if (!(inst->header.m_pIntelligence->m_dwFlags & layers)) {
                continue;
            }
            if (gCameraVisibleBox.m_vMax.x > inst->m_Bounds.m_vMin.x && gCameraVisibleBox.m_vMin.x < inst->m_Bounds.m_vMax.x
                && gCameraVisibleBox.m_vMax.z > inst->m_Bounds.m_vMin.z && gCameraVisibleBox.m_vMin.z < inst->m_Bounds.m_vMax.z
                && gCameraVisibleBox.m_vMax.y > inst->m_Bounds.m_vMin.y && gCameraVisibleBox.m_vMin.y < inst->m_Bounds.m_vMax.y) {
                CStaticObject__TouchLitVertices(inst);
                if (GameObject_IsSolid(inst) && world->m_nDrawActors != 0x200) {
                    world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)inst;
                }
            }
        }
        list = CISet__GetBlock(data, 1);
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
            if (gLitVerticesKeyChanged != 0) {
                pos.x = actor->header.m_vPos.x;
                pos.y = actor->header.m_vPos.y + GameObject_GetCollisionHeight(actor) * 0.5f;
                pos.z = actor->header.m_vPos.z;
                CScene__GetLightsAt(world, &pos, &actor->m_Lights);
            }
            if (gCameraVisibleBox.m_vMax.x > actor->m_Bounds.m_vMin.x && gCameraVisibleBox.m_vMin.x < actor->m_Bounds.m_vMax.x
                && gCameraVisibleBox.m_vMax.z > actor->m_Bounds.m_vMin.z && gCameraVisibleBox.m_vMin.z < actor->m_Bounds.m_vMax.z
                && gCameraVisibleBox.m_vMax.y > actor->m_Bounds.m_vMin.y && gCameraVisibleBox.m_vMin.y < actor->m_Bounds.m_vMax.y) {
                CLevelActor__Update(actor);
                if (world->m_nDrawActors != 0x200) {
                    world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)actor;
                }
            }
        }
    }
    CLevelActorPool__CollectVisible(&world->m_ActorPool, world);
}
