#include "common.h"
#include "structs/game_object.h"

typedef struct ObjDef {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x14];
    /* 0x18 */ u16 shape;
} ObjDef;


f32 func_0026BE60(void *node, f32 x, f32 z);
f32 func_002425A4(void *actor, f32 x, f32 z);
void func_00268798(CGameObjectInstance *actor, CVector3 *velocity);

/* func_00268688, inlined: floor height under (x, z) for an actor. */
static __inline__ f32 actorFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    ObjDef *def;

    if (actor->header.m_Type == 1) {
        def = ((ObjDef *)actor->header.m_pIntelligence);
        if (def->type == 2) {
            switch (def->shape) {
                case 0:
                    if (actor->header.m_pCurrentRegion != NULL) {
                        return func_0026BE60(actor->header.m_pCurrentRegion, x, z);
                    }
                    return actor->header.m_vPos.y;
                case 1:
                case 2:
                    return func_002425A4(actor, x, z);
            }
        }
    }
    return actor->header.m_vPos.y;
}

/* The platform an actor stands on, if it is still touching it. */
CGameObjectInstance *func_00267800(CGameObjectInstance *actor) {
    CGameObjectInstance *platform;
    CVector3 velocity;
    f32 floor;
    f32 gap;

    platform = NULL;
    if (actor->header.m_Type == 1) {
        platform = actor->m_pInstanceBelow;
    }
    if (platform == NULL) {
        return NULL;
    }
    if (((ObjDef *)platform->header.m_pIntelligence)->type != 2) {
        return platform;
    }
    floor = actorFloor(platform, actor->header.m_vPos.x, actor->header.m_vPos.z);
    func_00268798(platform, &velocity);
    gap = actor->header.m_vPos.y - floor + velocity.y;
    if ((gap < 0.0f) ? (-gap <= 5.12f) : (gap <= 5.12f)) {
        return platform;
    }
    return NULL;
}
