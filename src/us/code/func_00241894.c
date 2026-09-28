#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;
typedef struct { f32 x, y, z; } V3f;

extern s32 *D_800F1ADC;

void func_00241894(void) {
    s32 *p = D_800F1ADC;

    p[0] = 0;
    p[5] = -1;
    p[0x22] = 0;
    p[0x27] = 0;
    p[0x2C] = 0;
    p[0x2D] = 0;
    p[0x31] = 0;
}
