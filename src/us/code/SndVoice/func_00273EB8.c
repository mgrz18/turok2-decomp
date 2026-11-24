#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ s32 unkA8;
} Owner;

void SndVoice_SetSoundId(Owner *arg0, s32 arg1) {
    arg0->unkA8 = arg1;
}
