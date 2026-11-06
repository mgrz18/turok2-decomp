#include "common.h"
#include "m2c_macros.h"

extern s32 gRandomValue;
extern s32 gRandomValueB;

void Random_Seed(void) {
    gRandomValue = 0xE3F635E3;
    gRandomValueB = 0xE3F635E3;
}
