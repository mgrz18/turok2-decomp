#include "common.h"
#include "structs/sound.h"

void Sound__SetAmbientTrack(s32 arg0);

void Event_SetAmbient(s32 arg0, s32 arg1, SoundEffectInfo *arg2) {
    Sound__SetAmbientTrack(arg2->flags);
}
