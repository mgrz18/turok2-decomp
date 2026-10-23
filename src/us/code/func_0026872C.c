#include "common.h"

typedef struct {
    f32 x, y, z;
} Point3;

typedef struct {
    u8 pad0[0x4];
    Point3 pos;  /* 0x4 */
    void *node;  /* 0x10 */
} Placed;

f32 func_0026BE60(void *node, f32 x, f32 z);

f32 func_0026872C(Placed *obj, Point3 *offset) {
    void *node = obj->node;
    f32 result;

    if (node == NULL) {
        result = obj->pos.y;
    } else if (offset == NULL) {
        result = func_0026BE60(node, obj->pos.x, obj->pos.z);
    } else {
        result = func_0026BE60(node, obj->pos.x + offset->x, obj->pos.z + offset->z);
    }
    return result;
}
