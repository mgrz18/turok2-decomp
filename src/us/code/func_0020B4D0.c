#include "common.h"
#include "m2c_macros.h"

s32 Model_Decompress(s32, s32, s32, s32);

void Model_DecompressCallbackExtra(s32 arg0, s32 arg1) {
    Model_Decompress(arg0, arg1, 1, 0);
}
