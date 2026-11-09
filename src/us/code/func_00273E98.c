#include "common.h"
#include "structs/sound.h"

void SndVoice_SetPosition(SndVoice *arg0, CVector3 *arg1, sptr arg2) {
    arg0->vp.pos = *arg1;
    arg0->vp.follow = arg2;
}
