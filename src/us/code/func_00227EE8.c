#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

/* Spawns a copy of an actor: takes a free instance off the world's free list
 * and initialises it from src. */

extern void func_00219BC0(CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence,
                          s32 sound, void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale,
                          CVector3 velocity, s32 extra);
extern s32 func_00224180(void *level, s32 type);
extern void func_0021E5D4(CGameObjectInstance *obj);
extern void CList__AddHead(void *list, void *node);
extern void CList__Remove(void *list, void *node);
extern s32 D_800B6D00;
extern s32 D_800B6D54;
extern u8 D_800F7078[];
extern s32 D_8011001C;

/* func_002283FC without the owner counter. */
static __inline__ CGameObjectInstance *take_free_instance(void *world) {
    CGameObjectInstance *node;

    if (D_8011001C != 0 || M2C_FIELD(world, u32 *, 0x5324) < 3) {
        node = M2C_FIELD(world, CGameObjectInstance **, 0x5300);
        if (node != NULL) {
            CList__Remove((u8 *)world + 0x5300, node);
            CList__AddHead((u8 *)world + 0x5314, node);
            M2C_FIELD(node, s32 *, 0x290) = 0;
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

CGameObjectInstance *func_00227EE8(void *world, CGameObjectInstance *src) {
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
    if ((model == -1 && (model = func_00224180(D_800F7078, type)) == -1)
        || (obj = take_free_instance(world)) == NULL) {
        return NULL;
    }
    func_00219BC0(obj, model, type, D_800B6D00, intelligence, -1, region, rotY, d.pos, 0x80000, d.scale,
                  d.velocity, 0);
    func_0021E5D4(obj);
    return obj;
}
