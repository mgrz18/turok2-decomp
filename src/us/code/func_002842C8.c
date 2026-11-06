#include "common.h"
#include "m2c_macros.h"

extern s32 gCinemaEnabled;

void Cinema_Disable(void) {
    gCinemaEnabled = 0;
}
