#include "common.h"
#include "m2c_macros.h"

extern s32 gTotemDefense_kills[]; /* local type: gTotemDefense.kills (s32, globals/boss.h); the array keeps the ROM's code */

void TotemDefense_AddKill(void) {
    gTotemDefense_kills[0] += 1;
}
