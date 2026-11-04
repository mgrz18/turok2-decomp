#include "common.h"

/* What this reads out of a resource: two words and a pointer. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 *data;
} ResourceView;

s32 *func_002017D4(s32 *, s32);                     /* extern */

void func_002079D8(s32 **arg0, ResourceView *arg1) {
    s32 *temp_s0;

    temp_s0 = *arg0;
    if (*temp_s0 != 0) {
        arg1->data = func_002017D4(temp_s0, 0);
        arg1->unk0 = (s32) *func_002017D4(temp_s0, 1);
        arg1->unk4 = (s32) *func_002017D4(temp_s0, 2);
        return;
    }
    arg1->data = NULL;
    arg1->unk0 = 0;
    arg1->unk4 = 0;
}
