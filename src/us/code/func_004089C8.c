#include "common.h"
#include "m2c_macros.h"

extern s32 gTotemDefense_kills[];

void TotemDefense_AddKill(void) {
    gTotemDefense_kills[0] += 1;
}
