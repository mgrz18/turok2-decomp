#include "common.h"
#include "m2c_macros.h"

extern s32 D_800DD784;
extern s32 D_800DD788;

void Random_Seed(void) {
    D_800DD784 = 0xE3F635E3;
    D_800DD788 = 0xE3F635E3;
}
