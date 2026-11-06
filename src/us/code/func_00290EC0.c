#include "common.h"
#include "m2c_macros.h"

typedef struct { s16 type; s16 pad; s16 val; s16 pad2; s32 pad3[2]; } Msg;
extern void alEvtqPostEvent(void *, Msg *, s32);

void alCSPSetVol(u8 *arg0, s16 arg1) {
    Msg m;

    m.type = 0xA;
    m.val = arg1;
    alEvtqPostEvent(arg0 + 0x48, &m, 0);
}
