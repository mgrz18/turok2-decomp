#include "common.h"
#include "structs/event.h"

s32 Sound__Play2D(s32 arg0);

void Event_PlaySoundRemote(LevelEvent *arg0) {
    Sound__Play2D(arg0->sound);
}
