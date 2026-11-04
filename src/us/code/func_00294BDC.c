#include "common.h"

/* A list owner: the node list head at 0x14, the list itself (passed to func_002933E0) at 4. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u8 list[0x10];
    /* 0x14 */ s32 head;
} ListOwner294;

s32 func_002933E0(s32, s32);

s32 func_002933B0(s32);                         /* extern */

void func_00294BDC(ListOwner294 *arg0) {
    s32 var_s0;
    ListOwner294 *var_s1;

    var_s1 = arg0;
    var_s0 = var_s1->head;
    if (var_s0 != 0) {
        do {
            func_002933B0(var_s0);
            func_002933E0(var_s0, (s32) var_s1->list);
            var_s0 = var_s1->head;
        } while (var_s0 != 0);
    }
}
