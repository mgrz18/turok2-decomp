#include "common.h"

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ u8 pad4[0xB0 - 0x4];
    /* 0xB0 */ s32 header;
} Model;

s32 alSndpSetPan(s32, s32);
s32 alSndpSetSound(s32, s16);                    /* extern */

void SndVoice_SetPan(Model *arg0, s32 arg1) {
    sptr var_s0;
    s32 var_s1;
    s16 *temp_v0;

    var_s1 = arg1;
    var_s0 = arg0->header;
    temp_v0 = (s16 *) (var_s0 + (arg0->index * 2));
    var_s0 += 0x84;
    alSndpSetSound(var_s0, temp_v0[0xDC / 2]);
    alSndpSetPan(var_s0, var_s1 & 0xFF);
}
