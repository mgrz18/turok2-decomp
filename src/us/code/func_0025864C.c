#include "common.h"

void CAI__SetMode();                            /* extern */

void WeaponActor_SetModeWrapper(void) {
    CAI__SetMode();
}
