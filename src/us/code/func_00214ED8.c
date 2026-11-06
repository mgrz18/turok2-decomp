#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"


s32 Instance_NextUniqueId(void) {
    s32 temp_v0;

    temp_v0 = gNextUniqueId + 1;
    gNextUniqueId = temp_v0;
    if (temp_v0 == 0x3FFFFF) {
        gNextUniqueId = 0x380000;
    }
    return gNextUniqueId;
}
