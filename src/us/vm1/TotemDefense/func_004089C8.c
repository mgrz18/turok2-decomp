#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

void TotemDefense_AddKill(void) {
    s32 *kills = &gTotemDefense.kills; /* through a pointer: `gTotemDefense.kills++` loads it with %lo, the ROM builds the address first */

    *kills += 1;
}
