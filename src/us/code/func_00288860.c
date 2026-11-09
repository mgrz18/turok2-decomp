#include "common.h"
#include "structs/game.h"
#include "globals/engine.h"

void CEngineApp__Main(void *);                   /* extern */
s32 __osGetFpcCsr();                                /* extern */
void __osSetFpcCsr(s32);                         /* extern */
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);
#else
void sprintf(void *, void *, void *, void *); /* extern */
#endif

void mainproc(void) {
    sprintf(gEngineTimestamp, "%s %s", "Nov 10 1998", "13:17:04");
    __osSetFpcCsr(__osGetFpcCsr() | 0x400);
    CEngineApp__Main(&gEngineApp);
}
