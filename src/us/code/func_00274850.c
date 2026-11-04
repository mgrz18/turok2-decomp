#include "common.h"
#include "structs/sound.h"

s32 func_00293EC0(s32);
s32 func_00293580(void *);                          /* extern */
void func_00293E60(void *, s16);                    /* extern */

void func_00274850(SndVoice *arg0) {
    SndMgr *temp_a0;
    void *temp_s0;

    temp_a0 = arg0->mgr;
    arg0->stopping = 1;
    arg0->vp.follow = 0;
    temp_s0 = temp_a0->player;
    if (arg0->vp.frame != temp_a0->frame) {
        func_00293E60(temp_s0, temp_a0->handles[arg0->index]);
        if (func_00293580(temp_s0) != 0) {
            func_00293EC0((s32) temp_s0);
        }
        arg0->sound = -1;
    }
}
