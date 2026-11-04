#include "common.h"
#include "structs/sound.h"

s32 func_00293E70(s32, s32);
s32 func_00293E60(s32, s16);                    /* extern */

void func_002746AC(SndVoice *arg0, s32 arg1) {
    SndMgr *temp_s0;
    s32 temp_s0_2;

    temp_s0 = arg0->mgr;
    temp_s0_2 = (s32) &temp_s0->player;
    func_00293E60(temp_s0_2, temp_s0->handles[arg0->index]);
    func_00293E70(temp_s0_2, (s32) (s16) (s32) ((f32) arg1 * arg0->volumeScale));
}
