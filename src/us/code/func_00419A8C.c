#include "common.h"

void Music_RestoreVolume();                            /* extern */
void Sound__EnableAmbientTrack();                            /* extern */
void LoopSounds_Resume(s32);                         /* extern */
s32 LoopSounds_Get();                                /* extern */

s32 PauseMenu_Resume(void) {
    LoopSounds_Resume(LoopSounds_Get());
    Music_RestoreVolume();
    Sound__EnableAmbientTrack();
    return 1;
}
