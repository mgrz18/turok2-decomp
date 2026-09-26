#include "common.h"

extern u8 D_800F677A[];

s32 func_00285980(s32 arg0) {
    return D_800F677A[arg0 * 4] & 1;
}
