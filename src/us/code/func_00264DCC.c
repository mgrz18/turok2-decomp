#include "common.h"
#include "m2c_macros.h"

extern s8 gThrowableCollisionInfo_m_InstanceBehavior;

void Throwable_IdleUpdate(void) {
    gThrowableCollisionInfo_m_InstanceBehavior = 2;
}
