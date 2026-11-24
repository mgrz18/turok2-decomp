#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

s16 WeightedTable_PickStatic(void *); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */

s16 MantidQueen_PickFromStep(void) {
    return WeightedTable_PickStatic(gMantidQueen.step->picks10);
}
