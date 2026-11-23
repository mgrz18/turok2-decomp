#include "common.h"
#include "structs/event.h"

s32 Music_SetTrack(u16);

/* The mode parameter this AI callback is handed: only the field it reads. */

void Event_SetMusicRemote(LevelEvent *arg0) {
    Music_SetTrack(arg0->param);
}
