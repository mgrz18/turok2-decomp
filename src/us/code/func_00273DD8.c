#include "common.h"
#include "structs/sound.h"

 /* size 0xCC */

s32 func_00293EC0(s32);

s32 func_00293580(void *);                          /* extern */
s32 func_00293E60(void *, s16);                 /* extern */

void func_00273DD8(s32 arg0, s16 arg1) {
    SndMgr *temp_a0;
    SndVoice *var_s0;
    void *var_s1;

    /* voice arg1 of the table at arg0 */
    var_s0 = (SndVoice *) ((arg1 * 0xCC) + arg0);
    var_s0 = (SndVoice *) &((SndVoicePool *) var_s0)->voices[0];
    temp_a0 = var_s0->mgr;
    var_s0->stopping = 1;
    var_s0->vp.follow = 0;
    var_s1 = temp_a0->player;
    if (var_s0->vp.frame != temp_a0->frame) {
        func_00293E60(var_s1, temp_a0->handles[var_s0->index]);
        if (func_00293580(var_s1) != 0) {
            func_00293EC0((s32) var_s1);
        }
        var_s0->sound = -1;
    }
}
