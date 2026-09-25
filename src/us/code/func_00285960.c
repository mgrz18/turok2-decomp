#include "common.h"

extern u8 D_800F677B[];

s32 func_00285960(s32 arg0) {
    return ((D_800F677B[arg0 * 4] >> 3) ^ 1) & 1;
}
