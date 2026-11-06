#include "common.h"

extern s32 **CGameObjectInstance__LoadGeometry(void);
extern u8 *CISet__GetBlock(void *base, s32 index);
extern void CCache__Unlock(s32 arg0, void *arg1);

s32 func_0021F1F8(void) {
    s32 **res;
    s32 value;

    value = -1;
    res = CGameObjectInstance__LoadGeometry();
    if (res != NULL) {
        value = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*res, 3), 0), 1)[0x66];
        CCache__Unlock(0, res);
    }
    return value;
}
