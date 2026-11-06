#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

s32 WeightedTable_PickStatic(void *table);

s16 MantidQueen_PickFromStep(void) {
    return WeightedTable_PickStatic(gMantidQueen.step->picks10);
}
