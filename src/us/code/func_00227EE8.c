#include "common.h"
#include "structs/game_object.h"
#include "structs/list.h"

/* Spawns a copy of an actor: takes a free instance off the world's free list
 * and initialises it from src. */

extern void CGameObjectInstance__Construct(CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence,
                          s32 sound, void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale,
                          CVector3 velocity, s32 extra);
extern s32 CScene__FindObjectType(void *level, s32 type);
extern void func_0021E5D4(CGameObjectInstance *obj);
extern void CList__AddHead(void *list, void *node);
extern void CList__Remove(void *list, void *node);
extern s32 D_800B6D00;
extern s32 D_800B6D54;
extern u8 gScene[];
extern s32 D_8011001C;

/* CInstancePool__Alloc without the owner counter. */
static __inline__ CGameObjectInstance *take_free_instance(CInstancePool *world) {
    CGameObjectInstance *node;

    if (D_8011001C != 0 || (u32)world->m_ActiveList.m_nCount < 3) {
        node = (CGameObjectInstance *)world->m_FreeList.m_pHead;
        if (node != NULL) {
            CList__Remove(&world->m_FreeList, node);
            CList__AddHead(&world->m_ActiveList, node);
            ((CListedInstance *)node)->m_pOwnerCount = 0;
        }
        return node;
    }
    return NULL;
}

/* Only the vectors are copied into it; the frame layout needs them in one
 * aggregate. */
typedef struct {
    /* 0x00 */ u8 unk00[0x14];
    /* 0x14 */ CVector3 pos;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ CVector3 velocity;
    /* 0x30 */ CVector3 scale;
} SpawnInfo;

CGameObjectInstance *CInstancePool__SpawnCopy(CInstancePool *world, CGameObjectInstance *src) {
    s32 kind = src->header.m_Type;
    s32 type = (u16)src->m_nTypeFlag;
    void *intelligence = src->header.m_pIntelligence;
    void *region = src->header.m_pCurrentRegion;
    f32 rotY = src->m_RotY;
    SpawnInfo d;
    s32 model;
    CGameObjectInstance *obj;

    d.pos = src->header.m_vPos;
    d.velocity = src->m_vVelocity;
    d.scale = src->m_vScale;
    model = kind;
    if (D_800B6D54 == 0) {
        return NULL;
    }
    if (intelligence == NULL) {
        return NULL;
    }
    if ((model == -1 && (model = CScene__FindObjectType(gScene, type)) == -1)
        || (obj = take_free_instance(world)) == NULL) {
        return NULL;
    }
    CGameObjectInstance__Construct(obj, model, type, D_800B6D00, intelligence, -1, region, rotY, d.pos, 0x80000, d.scale,
                  d.velocity, 0);
    func_0021E5D4(obj);
    return obj;
}
