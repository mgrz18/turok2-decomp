#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 sound;
} SoundEvent;

s32 func_002759C4(s32);
s32 func_0027580C(u16);                         /* extern */

void func_0026E3A8(s32 arg0, s32 arg1, SoundEvent *arg2) {
    if ((u16) arg2->sound >= 0x100U) {
        func_0027580C(arg2->sound);
        return;
    }
    func_002759C4((s32) arg2->sound);
}
