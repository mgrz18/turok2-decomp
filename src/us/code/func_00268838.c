#include "common.h"

typedef struct {
    f32 x, y, z;
} Point3;

typedef struct {
    u8 kind;     /* 0x0 */
    u8 pad1[0x3];
    Point3 pos;  /* 0x4 */
    void *node;  /* 0x10 */
    u8 pad14[0xC0];
    s32 flags;   /* 0xD4 */
} Placed;

f32 func_0026BE60(void *node, f32 x, f32 z);

/* func_0026872C, inlined here. */
static __inline__ f32 floorHeight(Placed *obj, Point3 *offset) {
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

/* Whether the actor is on (or within 0.4 above) the floor. */
s32 func_00268838(Placed *actor) {
    if (actor->kind == 1 && (actor->flags & 0x1000)) {
        return 1;
    }
    return actor->pos.y - floorHeight(actor, NULL) <= 0.4f;
}
