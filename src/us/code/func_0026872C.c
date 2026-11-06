#include "common.h"
#include "structs/vector.h"

typedef struct {
    u8 pad0[0x4];
    CVector3 pos;  /* 0x4 */
    void *node;  /* 0x10 */
} Placed;

f32 CGameRegion__GetHeight(void *node, f32 x, f32 z);

f32 GameObject_GetRegionFloorHeight(Placed *obj, CVector3 *offset) {
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
