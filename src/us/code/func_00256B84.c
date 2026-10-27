#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

/* The part of the object that the probe disturbs and puts back. */
typedef struct {
    s32 w[13];
} ObjectHead;   /* 0x34 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ u8 pad10[0x54 - 0x10];
    /* 0x54 */ f32 stepHeight;
} Object;

extern s32 D_800F1B40;   /* collision info */
extern f32 func_002671B4(Object *obj);   /* the object's height */
extern s32 func_00239F00(Object *obj, Vec3 to, void *info);

/* Probes a move of obj, raised by half its height plus its step height, to
 * *to; writes where it ends up back to *to and returns 1 if that differs
 * from the target. The object itself is left as it was. */
s32 func_00256B84(Object *obj, Vec3 *to) {
    ObjectHead saved;
    s32 blocked;

    saved = *(ObjectHead *)obj;
    obj->pos.y += func_002671B4(obj) * 0.5f;
    obj->pos.y += obj->stepHeight;
    func_00239F00(obj, *to, &D_800F1B40);
    blocked = 0;
    if (to->x != obj->pos.x || to->y != obj->pos.y || to->z != obj->pos.z) {
        blocked = 1;
    }
    *to = obj->pos;
    *(ObjectHead *)obj = saved;
    return blocked;
}
