#include "common.h"
#include "structs/boss.h"

s32 WeightedTable_PickStatic(void *table);

s16 MantidQueen_PickFromStep(void) {
    return WeightedTable_PickStatic(D_8012F5B4->picks10);
}
