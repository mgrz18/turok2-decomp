#include "common.h"
#include "m2c_macros.h"
#include "globals/pak.h"


s32 PakMenu_HasPort(void) {
    return ~gPakMenuPort != 0;
}
