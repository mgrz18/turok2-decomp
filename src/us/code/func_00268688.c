#include "common.h"

f32 func_0026BE60(void *node, f32 x, f32 z);
f32 func_002425A4(void *actor, f32 x, f32 z);

static __inline__ f32 nodeFloor(u8 *actor, f32 x, f32 z) {
    void *node = *(void **) (actor + 0x10);
    f32 result;

    if (node == NULL) {
        result = *(f32 *) (actor + 0x8);
    } else {
        result = func_0026BE60(node, x, z);
    }
    return result;
}

/* Floor height under (x, z) for the actor. */
f32 func_00268688(u8 *actor, f32 x, f32 z) {
    u8 *def;

    if (actor[0] == 1) {
        def = *(u8 **) (actor + 0x14);
        if (*(s32 *) def == 2) {
            switch (*(u16 *) (def + 0x18)) {
                case 0:
                    return nodeFloor(actor, x, z);
                case 1:
                case 2:
                    return func_002425A4(actor, x, z);
            }
        }
    }
    return *(f32 *) (actor + 0x8);
}
