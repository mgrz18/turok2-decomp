#include "common.h"

s32 __osContGetInitData(s32 *, s32);              /* extern */

void osContGetQuery(s32 arg0) {
    s32 sp10;

    __osContGetInitData(&sp10, arg0);
}
