#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

#ifdef TARGET_PORT
s16 WeightedTable_PickStatic(void *table); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
#else
s32 WeightedTable_PickStatic(void *table);
#endif

s16 MantidQueen_PickFromStep(void) {
    return WeightedTable_PickStatic(gMantidQueen.step->picks10);
}
