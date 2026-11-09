#include "common.h"
#include "protos/iset.h"

extern void GameObject_EmitHotPoints(s32 *arg0, s32 arg1, s32 arg2, void *arg3, s32 *arg4);

void func_0021EE44(u8 *actor, s32 *data, s32 arg2) {
    s32 *countp;
    s32 *table;
    s32 *entry;
    s32 count;
    s32 value;
    s32 i;
    s32 bit;
    s32 *params;

    countp = CISet__GetBlock(data, 2);
    table = CISet__GetBlock(data, 4);
    count = *countp;
    for (i = 0; i < count; i++) {
        bit = 1 << i;
        if (*(s32 *)(actor + 0x14C) & bit) {
            entry = CISet__GetBlock(table, i);
            value = entry[1];
            if (value != 0) {
                params = entry + 2;
                GameObject_EmitHotPoints(params, value, arg2, actor, CISet__GetBlock(data, 3));
            }
        }
    }
}
