#include "common.h"

s32 HostIo_GetSR();                                /* extern */
s32 HostIo_SetSR(s32);                         /* extern */

s32 HostIo_DisableInt(void) {
    s32 temp_v0;

    temp_v0 = HostIo_GetSR();
    HostIo_SetSR(temp_v0 & ~1);
    return temp_v0 & 1;
}
