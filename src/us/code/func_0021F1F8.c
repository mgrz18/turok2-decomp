#include "common.h"

extern s32 **func_0021F358(void);
extern u8 *func_002017D4(void *base, s32 index);
extern void func_002051F4(s32 arg0, void *arg1);

s32 func_0021F1F8(void) {
    s32 **res;
    s32 value;

    value = -1;
    res = func_0021F358();
    if (res != NULL) {
        value = func_002017D4(func_002017D4(func_002017D4(*res, 3), 0), 1)[0x66];
        func_002051F4(0, res);
    }
    return value;
}
