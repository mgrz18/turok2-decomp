#include "common.h"
#include "m2c_macros.h"

extern s32 alEvtqPostEvent(s32, void *, s32);

typedef struct {
    s16 kind;
    s16 pad;
    s32 a;
    s32 b;
    s32 c;
} Msg;

void alCSPStop(s32 arg0) {
    Msg m;

    m.kind = 0x11;
    alEvtqPostEvent(arg0 + 0x48, &m, 0);
}
