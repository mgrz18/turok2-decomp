#include "common.h"

void CAudioWorld__SetForce2D(void *, s32);              /* extern */
extern u8 D_800F2A00[];

void Sound__SetForce2D(s32 arg0) {
    CAudioWorld__SetForce2D(D_800F2A00, arg0);
}
