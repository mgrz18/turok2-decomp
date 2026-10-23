#include "common.h"

typedef struct Target {
    u8 pad0[0x20];
    f32 unk20;      /* 0x20 */
    u8 pad24[0x44 - 0x24];
    s32 flags;      /* 0x44 */
} Target;

typedef struct Def {
    u16 unk0;
    u16 flags;      /* 0x2 */
} Def;

typedef struct Obj {
    u8 pad0[8];
    f32 unk8;       /* 0x8 */
    u8 padC[4];
    Def *def;       /* 0x10 */
} Obj;

extern u8 D_800F7078[];
extern Target *func_0022425C(void *arg0, Def *def);

s32 func_00232684(Obj *obj) {
    Target *target;

    if (obj->def == NULL) {
        return 0;
    }
    if (!(obj->def->flags & 1)) {
        return 0;
    }
    target = func_0022425C(D_800F7078, obj->def);
    if (target == NULL) {
        return 0;
    }
    if (!(target->flags & 0x1000000)) {
        return 0;
    }
    return obj->unk8 <= target->unk20;
}
