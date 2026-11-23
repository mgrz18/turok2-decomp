#include "common.h"
#include "structs/event.h"

/* A key event fired by an animation or a script: its handler gets (object, AI, event). */

s32 Sound__SetAmbientTrack(u16);                         /* extern */

void Event_SetAmbientRemote(LevelEvent *arg0) {
    Sound__SetAmbientTrack(arg0->param);
}
