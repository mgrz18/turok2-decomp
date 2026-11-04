#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 (*callback)();
} Unk0042F650;

void func_0042F650(Unk0042F650 *arg0) {
    arg0->callback();
}
