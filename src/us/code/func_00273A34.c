#include "common.h"
#include "structs/sound.h"

void SndVoice_Construct(SndVoice *arg0, CAudioWorld *arg1, s32 arg2) {
    arg0->id = -1;
    arg0->unk8 = -1;
    arg0->vp.channel = -1;
    arg0->vp.unk30 = -1;
    arg0->unkB4 = -1;
    arg0->mgr = arg1;
    arg0->index = arg2;
    arg0->vp.priority = 0;
    arg0->vp.w4 = 0;
    arg0->vp.distSq = 0;
    arg0->vp.prevDistSq = 0;
    arg0->flags = 0;
    arg0->stopping = 0;
    arg0->hasTrack = 0;
    arg0->reset = 0;
    arg0->unkC4 = 1;
    arg0->vp.scale = 1.0f;
    arg0->vp.doppler = 1.0f;
    arg0->pitchScale = 1.0f;
}
