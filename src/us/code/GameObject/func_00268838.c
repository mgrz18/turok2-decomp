#include "common.h"
#include "structs/vector.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the members at their host offsets (round 11 C26: the N64 pads put the flags (0xD4) on host m_rpGeometryHeader/m_rpObjectHeader) */
typedef struct {
    union {
        struct { u8 kind; };
        struct { u8 _hpPos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; CVector3 pos; };
        struct { u8 _hpNode[__builtin_offsetof(CGameObjectInstance, header.m_pCurrentRegion)]; void *node; };
        struct { u8 _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; s32 flags; };
    };
} Placed;
#else
typedef struct {
    u8 kind;     /* 0x0 */
    u8 pad1[0x3];
    CVector3 pos;  /* 0x4 */
    void *node;  /* 0x10 */
    u8 pad14[0xC0];
    s32 flags;   /* 0xD4 */
} Placed;
#endif

f32 CGameRegion__GetHeight(void *node, f32 x, f32 z);

/* GameObject_GetRegionFloorHeight, inlined here. */
static __inline__ f32 floorHeight(Placed *obj, CVector3 *offset) {
    void *node = obj->node;
    f32 result;

    if (node == NULL) {
        result = obj->pos.y;
    } else if (offset == NULL) {
        result = CGameRegion__GetHeight(node, obj->pos.x, obj->pos.z);
    } else {
        result = CGameRegion__GetHeight(node, obj->pos.x + offset->x, obj->pos.z + offset->z);
    }
    return result;
}

/* Whether the actor is on (or within 0.4 above) the floor. */
s32 GameObject_IsNearFloor(Placed *actor) {
    if (actor->kind == 1 && (actor->flags & 0x1000)) {
        return 1;
    }
    return actor->pos.y - floorHeight(actor, NULL) <= 0.4f;
}
