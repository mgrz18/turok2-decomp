#include "common.h"
#include "m2c_macros.h"

s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern s32 D_800B8908[];

void __osSetGlobalIntMask(s32 arg0) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    D_800B8908[0] |= arg0;
    __osRestoreInt(temp_v0);
}
