#include "common.h"

typedef struct Target {
    u8 pad0[0x44];
    s32 flags;      /* 0x44 */
} Target;

typedef struct Def {
    u8 pad0[6];
    s8 unk6;        /* 0x6 */
} Def;

typedef struct Obj {
    u8 pad0[4];
    Def *def;       /* 0x4 */
} Obj;

extern u8 D_800F7078[];
extern Target *func_0022425C(void *arg0);

s32 func_0023C188(Obj *obj) {
    Target *target;

    target = func_0022425C(D_800F7078);
    if (target != NULL && (target->flags & 0x4000)) {
        return 0;
    }
    return obj->def->unk6;
}
