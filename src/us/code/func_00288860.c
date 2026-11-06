#include "common.h"
#include "structs/game.h"

void CEngineApp__Main(void *);                   /* extern */
s32 __osGetFpcCsr();                                /* extern */
void __osSetFpcCsr(s32);                         /* extern */
void sprintf(void *, void *, void *, void *); /* extern */
extern u8 gEngineTimestamp[];

void mainproc(void) {
    sprintf(gEngineTimestamp, "%s %s", "Nov 10 1998", "13:17:04");
    __osSetFpcCsr(__osGetFpcCsr() | 0x400);
    CEngineApp__Main(&gEngineApp);
}
