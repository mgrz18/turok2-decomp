#include "common.h"
#include "protos/ai.h"

s32 Turret_Aim();                                /* extern */

void Turret_FireUpdate(s32 arg0, s32 arg1) {
    if (Turret_Aim() == 0) {
        CAI__SetMode(arg0, arg1, 0x3E);
    }
}
