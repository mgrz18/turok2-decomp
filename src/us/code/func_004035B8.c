#include "common.h"
#include "m2c_macros.h"

void MantidQueen_SortSwap(s32 *arg0, s32 *arg1) {
    s32 temp_v1;

    temp_v1 = *arg0;
    *arg0 = *arg1;
    *arg1 = temp_v1;
}
