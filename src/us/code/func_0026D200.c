#include "common.h"
#include "m2c_macros.h"

u16 FloatToHalf(f32 arg0) {
    return *(u16 *)&arg0;
}
