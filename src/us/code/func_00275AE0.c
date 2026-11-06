#include "common.h"

s32 CAudioWorld__SetListener(s32 *, s32);              /* extern */
extern s32 D_800F2A00;

void Sound__SetListener(s32 arg0) {
    CAudioWorld__SetListener(&D_800F2A00, arg0);
}
