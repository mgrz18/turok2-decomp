#include "common.h"
#include "m2c_macros.h"

s32 Model_Decompress(sptr, sptr, s32, s32);

void Model_DecompressCallback(sptr arg0, sptr arg1) {
    Model_Decompress(arg0, arg1, 0, 0);
}
