#include "common.h"
#include "structs/collision.h"

s32 Destructible_IsHitExplosive(DamageEvent *arg0) {
    return arg0->flags & 0x400000;
}
