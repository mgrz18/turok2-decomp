#include "common.h"

s32 CList__AddHead(sptr, sptr);                /* extern */
#ifdef TARGET_PORT
s32 CList__Remove(sptr, sptr);
#else
s32 CList__Remove(s32);
#endif                         /* extern */

void CEmitterPool__ReleaseEmitter(s32 arg0, s32 arg1) {
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): the emitter (a1) passes through to CList__Remove on MIPS */
    CList__Remove(arg0 + 0x8B4, arg1);

#else
    CList__Remove(arg0 + 0x8B4);

#endif
    CList__AddHead(arg0 + 0x8A0, arg1);
}
