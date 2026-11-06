#include "common.h"
#include "m2c_macros.h"

extern s32 gCinemaEnabled;

s32 Cinema_IsEnabled(void) {
    return gCinemaEnabled;
}
