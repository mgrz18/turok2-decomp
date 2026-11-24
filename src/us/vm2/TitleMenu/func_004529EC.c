#include "common.h"
#include "protos/game.h"

void Music_RestoreVolume(void);
void Sound__EnableAmbientTrack();                            /* extern */
void LoopSounds_Resume(sptr);                         /* extern */

s32 PauseMenu_Resume_2(void) {
    LoopSounds_Resume(LoopSounds_Get());
    Music_RestoreVolume();
    Sound__EnableAmbientTrack();
    return 1;
}
