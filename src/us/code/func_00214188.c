#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 size;
} Entry;

Entry *CISet__GetBlock(s32 *, s32);                    /* extern */

s32 func_00214188(s32 *arg0) {
    s32 var_s0;
    s32 var_s1;
    Entry *temp_v0;

    var_s0 = 0;
    var_s1 = 0;
    if (*arg0 > 0) {
        do {
            temp_v0 = CISet__GetBlock(arg0, var_s0);
            var_s0 += 1;
            var_s1 += temp_v0->size;
        } while (var_s0 < *arg0);
    }
    return var_s1;
}
