#include "common.h"
#include "m2c_macros.h"

typedef struct { s16 type; s32 val; s32 pad[2]; } Msg;
extern void alEvtqPostEvent(void *, Msg *, s32);

void alCSPSetBank(u8 *arg0, s32 arg1) {
    Msg m;

    m.type = 0xE;
    m.val = arg1;
    alEvtqPostEvent(arg0 + 0x48, &m, 0);
}
