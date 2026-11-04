#include "common.h"
#include "structs/collision.h"

typedef struct Target {
    u8 pad0[0x44];
    s32 flags;      /* 0x44 */
} Target;

extern u8 D_800F7078[];
extern Target *func_0022425C(void *arg0);

s32 func_0023C188(Sweep *sweep) {
    Target *target;

    target = func_0022425C(D_800F7078);
    if (target != NULL && (target->flags & 0x4000)) {
        return 0;
    }
    return sweep->def->m_GroundBehavior;
}
