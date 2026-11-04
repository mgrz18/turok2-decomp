#include "common.h"
#include "structs/sound.h"

/* Clears every channel's state. */
void func_00273E78(SndVoicePool *arg0) {
    s32 var_v0;
    SndVoice *var_a0;

    var_a0 = arg0->voices;
    var_v0 = 0x10;
    do {
        var_a0->vp.state = 0;
        var_v0 -= 1;
        var_a0++;
    } while (var_v0 >= 0);
}
