#include "common.h"

s32 CAudioWorld__SetMuted(s32 *, s32);              /* extern */
extern s32 D_800F2A00;

void Sound__SetMuted(s32 arg0) {
    CAudioWorld__SetMuted(&D_800F2A00, arg0);
}
