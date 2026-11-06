#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

s32 Cinema_RequestById();                                /* extern */

s32 Cinema_SetNextCallbacks(s32 arg0, void (*arg1)(Cinema *), void (*arg2)()) {
    s32 temp_s3;

    temp_s3 = Cinema_RequestById();
    if (gCinemaPlayerP->scriptId == arg0) {
        gCinemaPlayerP->nextOnClose = arg1;
        gCinemaPlayerP->nextOnEnd = arg2;
    } else {
        if (arg1 != NULL) {
            arg1(gCinemaPlayerP);
        }
        if (arg2 != NULL) {
            arg2();
        }
    }
    return temp_s3;
}
