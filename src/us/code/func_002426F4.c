#include "common.h"

typedef struct Mover {
    char pad0[0x140];
    s32 flags;         /* 0x140 */
    char pad144[0x25C - 0x144];
    f32 angle;         /* 0x25C */
} Mover;

typedef struct Collider {
    u8 kind;           /* 0x00 */
    char pad1[0x1C - 0x1];
    f32 unk1C;         /* 0x1C */
    char pad20[0x30 - 0x20];
    Mover *mover;      /* 0x30 */
    char pad34[0xD4 - 0x34];
    s32 flags;         /* 0xD4 */
} Collider;

f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
s32 func_0023AB18(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, f32 arg5, s32 arg6);

s32 func_002426F4(void *arg0, Mover *mover, Collider *collider, char *arg3) {
    f32 scale;

    scale = func_002982F0(mover->angle);
    scale *= func_002974C0(mover->angle);
    if (func_0023AB18(arg0, arg3 + 0x30, arg3, 0, 2, scale, 0) != 0) {
        mover->flags |= 0x10000;
        if (collider->kind == 1 && (collider->flags & 0x300000)) {
            mover->flags |= 0x20000;
        }
        collider->mover = mover;
        if (collider->unk1C < 0.0f) {
            collider->unk1C = 0.0f;
        }
        return 1;
    }
    return 0;
}
