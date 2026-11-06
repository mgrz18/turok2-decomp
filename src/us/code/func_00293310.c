#include "common.h"
#include "m2c_macros.h"

u8 __alSeqReadByte(u8 **arg0) {
    return *arg0[2]++;
}
