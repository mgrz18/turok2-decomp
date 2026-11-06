#include "common.h"

s32 CAudioWorld__StopHandle(s32 *, s32);              /* extern */
extern s32 D_800F2A00;

void Sound__Stop(s32 arg0) {
    CAudioWorld__StopHandle(&D_800F2A00, arg0);
}
