#include "common.h"
#include "structs/game_object.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct ObjDef {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x14];
    /* 0x18 */ u16 shape;
} ObjDef;


f32 func_0026BE60(void *node, f32 x, f32 z);
f32 func_002425A4(void *actor, f32 x, f32 z);

static __inline__ f32 nodeFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    void *node = actor->header.m_pCurrentRegion;
    f32 result;

    if (node == NULL) {
        result = actor->header.m_vPos.y;
    } else {
        result = func_0026BE60(node, x, z);
    }
    return result;
}

/* func_00268688, inlined: floor height under (x, z) for an actor. */
static __inline__ f32 actorFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    ObjDef *def;

    if (actor->header.m_Type == 1) {
        def = ((ObjDef *)actor->header.m_pIntelligence);
        if (def->type == 2) {
            switch (def->shape) {
                case 0:
                    return nodeFloor(actor, x, z);
                case 1:
                case 2:
                    return func_002425A4(actor, x, z);
            }
        }
    }
    return actor->header.m_vPos.y;
}

static __inline__ CGameObjectInstance *standOn(CGameObjectInstance *actor) {
    CGameObjectInstance *result = NULL;

    if (actor->header.m_Type == 1) {
        result = actor->m_pInstanceBelow;
    }
    return result;
}

/* Ground height under an actor: its own region floor, or what it stands on if higher. */
f32 func_00267648(CGameObjectInstance *actor) {
    f32 ground;

    if (actor->header.m_pCurrentRegion == NULL) {
        return actor->header.m_vPos.y;
    }
    ground = func_0026BE60(actor->header.m_pCurrentRegion, actor->header.m_vPos.x, actor->header.m_vPos.z);
    if (standOn(actor) == NULL) {
        return ground;
    }
    return MAX(ground, actorFloor(standOn(actor), actor->header.m_vPos.x, actor->header.m_vPos.z));
}
