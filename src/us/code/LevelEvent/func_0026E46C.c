#include "common.h"
#include "structs/event.h"

/* A key event fired by an animation or a script: its handler gets (object, AI, event). */

void Sound__SetAmbientTrack(s32 arg0);

void Event_SetAmbientRemote(LevelEvent *arg0) {
    Sound__SetAmbientTrack(arg0->param);
}
