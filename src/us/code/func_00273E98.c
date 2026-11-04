#include "common.h"
#include "structs/sound.h"

void func_00273E98(SndVoice *arg0, CVector3 *arg1, s32 arg2) {
    arg0->vp.pos = *arg1;
    arg0->vp.follow = arg2;
}
