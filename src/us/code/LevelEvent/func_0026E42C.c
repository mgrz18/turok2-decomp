#include "common.h"
#include "structs/event.h"

void Sound__Play2D(s16);                            /* extern */

void Event_PlaySoundRemote(LevelEvent *arg0) {
    Sound__Play2D(arg0->sound);
}
