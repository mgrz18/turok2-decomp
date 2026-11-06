#include "common.h"

s32 CAI__SetMode(s32, s32, s32);       /* extern */
s32 Turret_Aim();                                /* extern */

void Turret_AimUpdate(s32 arg0, s32 arg1) {
    if (Turret_Aim() != 0) {
        CAI__SetMode(arg0, arg1, 0x3F);
    }
}
