#include "common.h"
#include "structs/event.h"

/* A key event fired by an animation or a script: its handler gets (object, AI, event). */

s32 Sound__Play2D(s32);

void Event_PlaySound(s32 arg0, s32 arg1, LevelEvent *arg2) {
    Sound__Play2D((s16) arg2->param);
}
