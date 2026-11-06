#include "common.h"
#include "m2c_macros.h"

s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern s32 __OSGlobalIntMask[];

void __osSetGlobalIntMask(s32 arg0) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    __OSGlobalIntMask[0] |= arg0;
    __osRestoreInt(temp_v0);
}
