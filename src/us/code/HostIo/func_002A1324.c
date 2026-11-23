#include "common.h"

s32 HostIo_GetSR();                                /* extern */
s32 HostIo_SetSR(s32);                         /* extern */

void HostIo_RestoreInt(s32 arg0) {
    HostIo_SetSR(HostIo_GetSR() | arg0);
}
