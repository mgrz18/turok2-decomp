#include "common.h"
#include "rw/abu_protos.h"

/* Something whose current state is a handler address at 0x24. */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 stateHandler;
} StateHolder;

extern s32 D_0043BDF0;
extern s32 D_0043BE14;
extern s32 D_80130940;

/* Whether the state handler is one of the two at 0x43BE14 / 0x43BDF0 (and clears D_80130940 then). */
s32 func_0045A574(StateHolder *arg0) {
    s32 temp_v1;

    temp_v1 = arg0->stateHandler;
    if ((temp_v1 == (s32) &D_0043BE14) || (temp_v1 == (s32) &D_0043BDF0)) {
        D_80130940 = 0;
        return 1;
    }
    return 0;
}
