#include "common.h"
#include "m2c_macros.h"

extern char D_800A5AC4[];

void **func_0020367C(s32, s32, s32, s32, s32, s32, void *, s32); /* resource load */
void func_002051F4(s32, void **);                                 /* resource free */
void **func_0021F358(void *obj);
s32 func_002017D4(void *, s32);
s32 func_0021CDAC(void *, void *, s32);

s32 func_0021EAE8(void *obj, s32 arg1) {
    s32 result;
    void **res;
    void **sub;
    void *first;

    result = 0;
    if (!(M2C_FIELD(obj, s32 *, 0xD4) & 0x40000)) {
        return result;
    }
    res = func_0020367C(0, M2C_FIELD(obj, s32 *, 0xA4), M2C_FIELD(obj, s32 *, 0xB0), 0, 0, 0, D_800A5AC4, arg1);
    if (res != NULL) {
        sub = func_0021F358(obj);
        if (sub != NULL) {
            first = *sub;
            result = func_0021CDAC(obj, first, func_002017D4(*res, 0));
            func_002051F4(0, sub);
        }
        func_002051F4(0, res);
    }
    return result;
}
