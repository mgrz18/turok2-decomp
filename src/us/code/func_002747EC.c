#include "common.h"

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ u8 pad4[0xB0 - 0x4];
    /* 0xB0 */ s32 header;
} Model;

s32 alSndpGetState(sptr);                         /* extern */
s32 alSndpSetSound(sptr, s16);                    /* extern */

void SndVoice_GetState(Model *arg0) {
    sptr temp_s0;
    sptr temp_v1;

    temp_v1 = arg0->header;
    temp_s0 = temp_v1 + 0x84;
    alSndpSetSound(temp_s0, ((s16 *) (temp_v1 + (arg0->index << 1)))[0xDC / 2]);
    alSndpGetState(temp_s0);
}
